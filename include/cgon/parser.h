/*
	Copyright (c) 2018 chaoticgd

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#ifndef _CGON_PARSER_H
#define _CGON_PARSER_H

#include "parse_error.h"
#include "token.h"
#include "object.h"
#include "type_list.h"
#include "type_string.h"
#include "container_traits.h"

namespace cgon {

	template <typename T_head, typename T_tail>
	std::unique_ptr<object> parse_object(token_iterator& current);

	template <typename T_owner, typename T_head, typename T_tail>
	void parse_property(T_owner* owner, token_iterator& current);

	template <typename T>
	T parse_expression(token_iterator& current);

	template <typename T>
	std::unique_ptr<T> parse_object_of_type(token_iterator& current) {
		
		token_iterator type_name_iterator = current++;
		std::string type_name = type_name_iterator->value();

		std::unique_ptr<T> result = std::make_unique<T>();

		if((current++)->value() != "{") {
			result->set_name((current - 1)->value());

			if((current++)->value() != "{") {
				throw parse_error("Expected '{'", current - 1);
			}
		}

		while(current->value() != "}") {

			if((current + 1)->value() == "=") {
				parse_property<T, typename T::properties::head,
				                  typename T::properties::tail>(result.get(), current);
			} else {
				std::unique_ptr<object> new_child(
					parse_object<typename T::child_types::head,
					             typename T::child_types::tail>(current));
				result->append_child(new_child);
			}

		}

		current++; // Skip over '}'.

		return result;
	}

	template <typename T_head, typename T_tail>
	std::unique_ptr<object> parse_object(token_iterator& current) {
		
		if constexpr(!std::is_same<T_head, type_list_end>()) {
			if(current->value() == get_string<typename T_head::type_name>::value()) {
				return parse_object_of_type<T_head>(current);
			}
		}

		if constexpr(!std::is_same<T_tail, type_list_end>()) {
			return parse_object<typename T_tail::head,
			                    typename T_tail::tail>(current);
		}

		throw parse_error("Invalid type name", current);
	}

	template <typename T_owner, typename T_head, typename T_tail>
	void parse_property(T_owner* owner, token_iterator& current) {

		if constexpr(!std::is_same<T_head, type_list_end>()) {
			if(current->value() == get_string<typename T_head::name>::value()) {
				current += 2; // Skip over property name and '='.
				typename T_head::type value =
					parse_expression<typename T_head::type>(current);
				(owner->*T_head::_setter)(value);
				return;
			}
		}

		if constexpr(!std::is_same<T_tail, type_list_end>()) {
			parse_property<T_owner, typename T_tail::head,
			                        typename T_tail::tail>(owner, current);
			return;
		}

		throw parse_error("Invalid property", current);
	}

	template <typename T>
	T parse_expression(token_iterator& current) {

		if constexpr(is_vector<T>::value) {
			if((current++)->value() != "[") {
				throw parse_error("Expected '['", current - 1);
			}

			T result;

			while(current->value() != "]") {
				typename T::value_type value =
					parse_expression<typename T::value_type>(current);
				result.push_back(value);
			}

			current++; // Skip over ']'.

			return result;
		}

		if constexpr(std::is_integral<T>() || std::is_floating_point<T>()) {
			arithmetic_expression expression(current);
			return expression.value();
		}
		
		if constexpr(std::is_same<T, std::string>()) {
			string_expression expression(current);
			return expression.value();
		}
		
		throw std::runtime_error("Not yet implemented");
	}
}

#endif
