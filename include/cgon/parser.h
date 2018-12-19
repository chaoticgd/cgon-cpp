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
#include "type_string.h"
#include "container_traits.h"

namespace cgon {

	template <typename T_child_types, int T_index>
	std::unique_ptr<object> parse_object(token_iterator& current);

	template <typename T>
	std::unique_ptr<T> parse_object_of_type(token_iterator& current);

	template <typename T_owner, typename T_properties, int T_index>
	void parse_property(token_iterator& current, T_owner* owner);

	template <typename T_properties, int T_index>
	void validate_properties(token_iterator& current, std::vector<std::string_view> property_names);

	template <typename T>
	T parse_expression(token_iterator& current);

	template <typename T>
	T parse_vector(token_iterator& current);
	
	template <typename T>
	T parse_array(token_iterator& current);

	template <typename T>
	T parse_tuple(token_iterator& current);

	template <typename T_tuple, int T_index>
	void parse_tuple_element(token_iterator& current, T_tuple& tuple);

	template <typename T>
	T parse_arithmetic_expression(token_iterator& current);

	static bool parse_bool(token_iterator& current);

	static std::string parse_string(token_iterator& current);

	template <typename T_child_types, int T_index>
	std::unique_ptr<object> parse_object(token_iterator& current) {

		if constexpr(T_index < std::tuple_size<T_child_types>::value) {

			using child_type = typename
				std::tuple_element<T_index, T_child_types>::type;

			if(current->value() == get_string<typename child_type::type_name>::value()) {
				return parse_object_of_type<child_type>(current);
			}

			return parse_object<T_child_types, T_index + 1>(current);
		}

		throw parse_error("Invalid type name", current);
	}

	template <typename T>
	std::unique_ptr<T> parse_object_of_type(token_iterator& current) {
		
		token_iterator type_name_iterator = current++;
		std::string_view type_name = type_name_iterator->value();

		std::unique_ptr<T> result = std::make_unique<T>();

		if((current++)->value() != "{") {
			result->set_name((current - 1)->copy_value());

			if((current++)->value() != "{") {
				throw parse_error("Expected '{'", current - 1);
			}
		}

		std::vector<std::string_view> property_names;

		while(current->value() != "}") {

			if((current + 1)->value() == ":") {
				std::string_view property_name = current->value();
				if(std::find(property_names.begin(), property_names.end(),
				             property_name) != property_names.end()) {
					throw parse_error("Multiple definitions of property", current);
				} else {
					property_names.push_back(property_name);
				}
				parse_property<T, typename T::properties, 0>(current, result.get());
			} else if constexpr(std::tuple_size<typename T::child_types>::value > 0) {
				std::unique_ptr<object> new_child(
					parse_object<typename T::child_types, 0>(current));
				result->append_child(new_child);
			}

		}

		validate_properties<typename T::properties, 0>(current, property_names);

		current++; // Skip over '}'.

		return result;
	}

	template <typename T_owner, typename T_properties, int T_index>
	void parse_property(token_iterator& current, T_owner* owner) {

		if constexpr(T_index < std::tuple_size<T_properties>::value) {

			using property = typename
				std::tuple_element<T_index, T_properties>::type;
			
			if(current->value() == get_string<typename property::name>::value()) {
				current += 2; // Skip over property name and '='.
				typename property::type value =
					parse_expression<typename property::type>(current);
				property::set(owner, value);
				return;
			}

			parse_property<T_owner, T_properties, T_index + 1>(current, owner);
			return;
		}

		throw parse_error("Invalid property name", current);
	}

	template <typename T_properties, int T_index>
	void validate_properties(token_iterator& current, std::vector<std::string_view> property_names) {

		if constexpr(T_index < std::tuple_size<T_properties>::value) {

			using property = typename
				std::tuple_element<T_index, T_properties>::type;

			std::string property_name =
				get_string<typename property::name>::value();

			if(std::find(property_names.begin(), property_names.end(),
			             property_name) == property_names.end() &&
			   !is_optional<typename property::type>::value) {
				throw parse_error(std::string("Property '") + property_name + "' has not been defined", current);
			}

			validate_properties<T_properties, T_index + 1>(current, property_names);
		}
	}

	template <typename T>
	T parse_expression(token_iterator& current) {
		
		if constexpr(is_optional<T>::value) {
			return parse_expression<typename T::value_type>(current);
		} else if constexpr(is_vector<T>::value) {
			return parse_vector<T>(current);
		} else if constexpr(is_array<T>::value) {
			return parse_array<T>(current);
		} else if constexpr(is_tuple<T>::value) {
			return parse_tuple<T>(current);
		} else if constexpr(std::is_same<T, bool>()) {
			return parse_bool(current);
		} else if constexpr(std::is_integral<T>() || std::is_floating_point<T>()) {
			return parse_arithmetic_expression<T>(current);
		} else if constexpr(std::is_same<T, std::string>()) {
			return parse_string(current);
		} else {
			return T(current);
		}
	}

	template <typename T>
	T parse_vector(token_iterator& current) {
		if((current++)->value() != "[") {
			throw parse_error("Expected '['", current - 1);
		}

		T result;
		while(current->value() != "]") {
			typename T::value_type value = parse_expression<typename T::value_type>(current);
			result.push_back(value);
		}

		current++; // Skip over ']'.

		return result;
	}

	template <typename T>
	T parse_array(token_iterator& current) {
		if((current++)->value() != "[") {
			throw parse_error("Expected '['", current - 1);
		}

		T result;
		for(typename T::value_type& element : result) {
			element = parse_expression<typename T::value_type>(current);
		}

		if((current++)->value() != "]") {
			throw parse_error("Expected ']'", current - 1);
		}

		return result;
	}

	template <typename T>
	T parse_tuple(token_iterator& current) {
		if((current++)->value() != "(") {
			throw parse_error("Expected '('", current - 1);
		}

		T result;
		parse_tuple_element<T, 0>(current, result);

		if((current++)->value() != ")") {
			throw parse_error("Expected ')'", current - 1);
		}

		return result;
	}

	template <typename T_tuple, int T_index>
	void parse_tuple_element(token_iterator& current, T_tuple& tuple) {

		using element_type = typename std::tuple_element<T_index, T_tuple>::type;

		std::get<T_index>(tuple) =
			parse_expression<element_type>(current);

		if constexpr(T_index + 1 < std::tuple_size<T_tuple>::value) {
			parse_tuple_element<T_tuple, T_index + 1>(current, tuple);
		}

	}

	template <typename T>
	T parse_arithmetic_expression(token_iterator& current) {
		try {
			return std::stod((current++)->copy_value());
		} catch(...) {
			throw parse_error("Expected a number", current - 1);
		}
	}

	bool parse_bool(token_iterator& current) {
		std::string_view value = (current++)->value();
		if(value == "false") {
			return false;
		} else if(value == "true") {
			return true;
		} else {
			throw parse_error("Expected a boolean", current - 1);
		}
	}

	std::string parse_string(token_iterator& current) {
		std::string_view token_value = current->value();
		if((*(token_value.begin()) != '\'' || *(token_value.end() - 1) != '\'') &&
		   (*(token_value.begin()) != '\"' || *(token_value.end() - 1) != '\"')) {
			throw parse_error("String expected", current - 1);
		}
		return (current++)->copy_value().substr(1, token_value.size() - 2);
	}
}

#endif
