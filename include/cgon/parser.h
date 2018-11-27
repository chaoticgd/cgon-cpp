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

#include <stdexcept>

#include "token.h"
#include "object.h"
#include "type_list.h"

namespace cgon {

	template <typename T_this>
	std::unique_ptr<base_object> parse_object(token_iterator& current, token_iterator end);

	template <typename T_this, typename T_that, typename... T_rest>
	std::unique_ptr<base_object> parse_object(token_iterator& current, token_iterator end);

	class parse_error : public std::runtime_error {
	public:
		parse_error(const std::string& what, token_iterator where)
			: std::runtime_error(what),
			  _message(std::string(what) + ":\n\t" + token_get_line(where)) {}

		const char* what() const noexcept {
			return _message.c_str();
		}
	private:
		std::string _message;
	};

	template <typename T>
	std::unique_ptr<base_object> parse_object_of_type(token_iterator& current, token_iterator end) {
		
		token_iterator type_name_iterator = current++;
		std::string type_name = type_name_iterator->value();

		std::unique_ptr<base_object> result = std::make_unique<T>();

		if((current++)->value() != "{") {
			result->set_name((current - 1)->value());

			if((current++)->value() != "{") {
				throw parse_error("Expected '{'", current - 1);
			}
		}

		while(current < end) {

			if(current->value() == "}") {
				current++;
				break;
			}

			if((current + 1)->value() == "=") {
				std::string property_name = (current++)->value();

				if(!result->has_property(property_name)) {
					throw parse_error("The object does not contain the named property", current - 1);
				}

				base_property* property = result->get_property(property_name);

				current++; // Skip over "=".

				property->parse(current, end);
			} else {
				std::unique_ptr<base_object> new_child(
					parse_object<typename T::child_types::head,
					             typename T::child_types::tail>(current, end));
				result->append_child(new_child);
			}

		}

		for(base_property* property : result->properties()) {
			if(!property->is_ready()) {
				throw parse_error(std::string("The property '") + property->name() + "' of the object has not been set", type_name_iterator);
			}
		}

		return result;
	}

	template <typename T_this>
	std::unique_ptr<base_object> parse_object(token_iterator& current, token_iterator end) {
		throw parse_error("Invalid type name", current);
	}

	template <typename T_this, typename T_that, typename... T_rest>
	std::unique_ptr<base_object> parse_object(token_iterator& current, token_iterator end) {
		
		if constexpr(!std::is_same<T_this, type_list_end>()) {
			if(current->value() == T_this::type_name()) {
				return parse_object_of_type<T_this>(current, end);
			}
		}

		return parse_object<T_that, T_rest...>(current, end);
	}
}

#endif
