#ifndef _CGON_PARSER_H
#define _CGON_PARSER_H

#include <stdexcept>

#include "token.h"
#include "type_registrar.h"
#include "object.h"

namespace cgon {

	class parse_error : public std::runtime_error {
	public:
		parse_error(const std::string& what, token where)
			: std::runtime_error(what),
			  _message(std::string(what) + ": " + where.to_string()) {}

		const char* what() const noexcept {
			return _message.c_str();
		}
	private:
		std::string _message;
	};

	std::unique_ptr<base_object> parse_object(type_registrar<base_object> allowed_child_types, token_iterator& current, token_iterator end) {
		token type_name_token = *(current++);
		std::string type_name = type_name_token.value();
	
		if(!allowed_child_types.contains(type_name)) {
			throw parse_error("Invalid type name", type_name_token);
		}

		std::unique_ptr<base_object> result(allowed_child_types.get(type_name).create());

		while((current++)->value() != "{") {
			// result->set_property("id", (current++)->value());
		}

		while(current < end) {

			if(current->value() == "}") {
				current++;
				break;
			}

			if((current + 1)->value() == "=") {
				std::string name = current->value();
				std::string value = (current + 2)->value();
			} else {
				std::unique_ptr<base_object> new_child =
					std::move(parse_object(result->allowed_child_types(), current, end));
				result->append_child(new_child);
			}

		}

		return result;
	}
}

#endif