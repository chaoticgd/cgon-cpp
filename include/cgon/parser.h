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
#include "type_registrar.h"
#include "object.h"

namespace cgon {

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

	std::unique_ptr<base_object> parse_object(token_iterator& current, token_iterator end, type_registrar<base_object> allowed_child_types) {
		
		token_iterator type_name_iterator = current++;
		std::string type_name = type_name_iterator->value();
		if(!allowed_child_types.contains(type_name)) {
			throw parse_error("Invalid type name", type_name_iterator);
		}

		std::unique_ptr<base_object> result(allowed_child_types.get(type_name).create());

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

				if(property->is_integral()) {
					int number = std::stoi((current++)->value());
					result->set_property<int>(property_name, number);
				} else if(property->is_floating_point()) {
					double number = std::stod((current++)->value());
					result->set_property<double>(property_name, number);
				} else if(property->is_string()) {
					// ...
					throw std::runtime_error("not yet implemented");
				} else {
					std::unique_ptr<base_object> property_value =
						parse_object(current, end, property->get_type());
					// uhhh....
					throw std::runtime_error("not yet implemented");
				}
			} else {
				std::unique_ptr<base_object> new_child =
					std::move(parse_object(current, end, result->allowed_child_types()));
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
}

#endif
