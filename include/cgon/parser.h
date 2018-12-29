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

	template <typename T_language, typename T_child_types, std::size_t T_index>
	std::unique_ptr<object> parse_object(token_iterator& current);

	template <typename T_language, typename T>
	std::unique_ptr<T> parse_object_of_type(token_iterator& current);

	template <typename T_language, typename T_owner, typename T_properties, std::size_t T_index>
	void parse_property(token_iterator& current, const std::string& given_name, T_owner* owner);

	static void validate_property_uniqueness(token_iterator& current, const std::vector<std::string>& property_names, const std::string& name);

	template <typename T_language, typename T_properties, std::size_t T_index>
	void validate_properties_are_initialised(token_iterator& current, const std::vector<std::string>& property_names);

	template <typename T_language, typename T_property_list, typename T_owner>
	void parse_non_optional_property_list(token_iterator& current, T_owner* owner);

	template <typename T_language, typename T_property_list, typename T_owner, std::size_t T_index>
	void parse_non_optional_property_list_elements(token_iterator& current, T_owner* owner, std::vector<std::string>& property_names);

	template <typename T_language, typename T>
	T parse_expression(token_iterator& current);

	template <typename T_language, typename T>
	T parse_vector(token_iterator& current);
	
	template <typename T_language, typename T>
	T parse_array(token_iterator& current);

	template <typename T_language, typename T>
	T parse_tuple(token_iterator& current);

	template <typename T_language, typename T_tuple, std::size_t T_index>
	void parse_tuple_elements(token_iterator& current, T_tuple& tuple);

	template <typename T_language, typename T>
	T parse_arithmetic_expression(token_iterator& current);

	static bool parse_bool(token_iterator& current);

	template <typename T_language>
	std::string parse_string(token_iterator& current);

	template <typename T_language>
	void parse_delimiter(token_iterator& current);

	template <typename T_language, typename T_child_types, std::size_t T_index>
	std::unique_ptr<object> parse_object(token_iterator& current) {

		if constexpr(T_index < std::tuple_size_v<T_child_types>) {

			using child_type = std::tuple_element_t<T_index, T_child_types>;

			if(current->value() == get_string<typename child_type::type_name>::value()) {
				current++; // Skip over type name.
				return parse_object_of_type<T_language, child_type>(current);
			}

			return parse_object<T_language, T_child_types, T_index + 1>(current);
		}

		throw parse_error("Invalid type name", current);
	}

	template <typename T_language, typename T>
	std::unique_ptr<T> parse_object_of_type(token_iterator& current) {
		
		std::unique_ptr<T> result = std::make_unique<T>();

		if((current++)->value() != "{") {

			bool error = false;

			if constexpr(T_language::allow_named_objects) {
				result->set_name((current - 1)->copy_value());

				if((current++)->value() != "{") {
					error = true;
				}
			} else {
				error = true;
			}

			if(error) {
				throw parse_error("Expected '{'", current - 1);
			}
		}

		std::vector<std::string> property_names;

		while(current->value() != "}") {

			if constexpr(T_language::delimit_lists) {
				if(property_names.size() > 0) {
					parse_delimiter<T_language>(current);
				}
			}

			if((current + 1)->value() == ":") {
				std::string property_name = current->copy_value();
				if constexpr(T_language::quoted_property_names) {
					property_name = parse_string<T_language>(current);
				} else {
					current++;
				}

				validate_property_uniqueness(current, property_names, property_name);
				property_names.push_back(property_name);

				current++; // Skip over ':'.

				parse_property<T_language, T, typename T::properties, 0>
					(current, property_name, result.get());
					
			} else if constexpr(T_language::allow_children) {
				if constexpr(std::tuple_size_v<typename T::child_types> > 0) {
					std::unique_ptr<object> new_child(
						parse_object<T_language, typename T::child_types, 0>(current));
					result->append_child(new_child);
				}
			} else {
				throw parse_error("Expected property", current);
			}
		}

		validate_properties_are_initialised<T_language, typename T::properties, 0>(current, property_names);

		current++; // Skip over '}'.

		return result;
	}

	template <typename T_language, typename T_owner, typename T_properties, std::size_t T_index>
	void parse_property(token_iterator& current, const std::string& given_name, T_owner* owner) {

		if constexpr(T_index < std::tuple_size_v<T_properties>) {

			using property = std::tuple_element_t<T_index, T_properties>;
			
			if(given_name == get_string<typename property::name>::value()) {

				if constexpr(property::is_property_list) {
					if constexpr(property::template contains_optional<0>()) {
						throw parse_error("std::optional not yet implemented for property lists", current);
					} else {
						parse_non_optional_property_list<T_language, property, T_owner>(current, owner);
					}
				} else {
					typename property::type value =
						parse_expression<T_language, typename property::type>(current);
					property::set(owner, value);
				}

				return;
			}

			parse_property<T_language, T_owner, T_properties, T_index + 1>(current, given_name, owner);
			return;
		}

		throw parse_error("Invalid property name", current);
	}

	void validate_property_uniqueness(token_iterator& current, const std::vector<std::string>& property_names, const std::string& name) {
		if(std::find(property_names.begin(), property_names.end(),
		             name) != property_names.end()) {
			throw parse_error(std::string("Multiple definitions of property '") + name + "'", current - 1);
		}
	}

	template <typename T_language, typename T_properties, std::size_t T_index>
	void validate_properties_are_initialised(token_iterator& current, const std::vector<std::string>& property_names) {

		if constexpr(T_index < std::tuple_size_v<T_properties>) {

			using property = std::tuple_element_t<T_index, T_properties>;

			std::string property_name =
				get_string<typename property::name>::value();

			if(std::find(property_names.begin(), property_names.end(),
			             property_name) == property_names.end() &&
			   !is_optional<typename property::type>::value) {
				throw parse_error(std::string("Property '") + property_name + "' has not been defined", current);
			}

			validate_properties_are_initialised<T_language, T_properties, T_index + 1>(current, property_names);
		}
	}

	template <typename T_language, typename T_property_list, typename T_owner>
	void parse_non_optional_property_list(token_iterator& current, T_owner* owner) {

		if((current++)->value() != "{") {
			throw parse_error("Expected '{'", current - 1);
		}

		std::vector<std::string> property_names;
		parse_non_optional_property_list_elements<T_language, T_property_list, T_owner, 0>
			(current, owner, property_names);
		
		if((current++)->value() != "}") {
			throw parse_error("Expected '}'", current - 1);
		}
	}

	template <typename T_language, typename T_property_list, typename T_owner, std::size_t T_index>
	void parse_non_optional_property_list_elements(token_iterator& current, T_owner* owner, std::vector<std::string>& property_names) {

		if constexpr(T_language::delimit_lists) {
			if(property_names.size() > 0) {
				parse_delimiter<T_language>(current);
			}
		}

		std::string property_name = parse_string<T_language>(current);

		if((current++)->value() != ":") {
			throw parse_error("Expected ':'", current - 1);
		}

		validate_property_uniqueness(current, property_names, property_name);
		property_names.push_back(property_name);

		parse_property<T_language, T_owner, typename T_property_list::type, 0>
			(current, property_name, owner);

		if constexpr(T_index + 1 < std::tuple_size_v<typename T_property_list::type>) {
			parse_non_optional_property_list_elements<T_language, T_property_list, T_owner, T_index + 1>
				(current, owner, property_names);
		}
	}

	template <typename T_language, typename T>
	T parse_expression(token_iterator& current) {
		
		if constexpr(is_optional<T>::value) {
			return parse_expression<T_language, typename T::value_type>(current);
		} else if constexpr(is_vector<T>::value) {
			return parse_vector<T_language, T>(current);
		} else if constexpr(is_array<T>::value) {
			return parse_array<T_language, T>(current);
		} else if constexpr(is_tuple<T>::value) {
			return parse_tuple<T_language, T>(current);
		} else if constexpr(std::is_same<T, bool>()) {
			return parse_bool(current);
		} else if constexpr(std::is_integral<T>() || std::is_floating_point<T>()) {
			return parse_arithmetic_expression<T_language, T>(current);
		} else if constexpr(std::is_same<T, std::string>()) {
			return parse_string<T_language>(current);
		} else if constexpr(is_unique_ptr<T>::value) {
			return parse_object_of_type<T_language, typename T::element_type>(current);
		} else {
			return T(current);
		}
	}

	template <typename T_language, typename T>
	T parse_vector(token_iterator& current) {
		if((current++)->value() != "[") {
			throw parse_error("Expected '['", current - 1);
		}

		T result;
		while(current->value() != "]") {

			if(result.size() != 0) {
				parse_delimiter<T_language>(current);
			}

			typename T::value_type element =
				parse_expression<T_language, typename T::value_type>(current);
			result.push_back(std::move(element));
		}

		current++; // Skip over ']'.

		return result;
	}

	template <typename T_language, typename T>
	T parse_array(token_iterator& current) {
		if((current++)->value() != "[") {
			throw parse_error("Expected '['", current - 1);
		}

		T result;
		bool first_element = true;
		for(typename T::value_type& element : result) {

			if(first_element) {
				parse_delimiter<T_language>(current);
				first_element = false;
			}

			element = parse_expression<T_language, typename T::value_type>(current);
		}

		if((current++)->value() != "]") {
			throw parse_error("Expected ']'", current - 1);
		}

		return result;
	}

	template <typename T_language, typename T>
	T parse_tuple(token_iterator& current) {
		if((current++)->value() != "(") {
			throw parse_error("Expected '('", current - 1);
		}

		T result;
		parse_tuple_elements<T_language, T, 0>(current, result);

		if((current++)->value() != ")") {
			throw parse_error("Expected ')'", current - 1);
		}

		return result;
	}

	template <typename T_language, typename T_tuple, std::size_t T_index>
	void parse_tuple_elements(token_iterator& current, T_tuple& tuple) {

		using element_type = std::tuple_element_t<T_index, T_tuple>;

		std::get<T_index>(tuple) =
			parse_expression<T_language, element_type>(current);

		if constexpr(T_index + 1 < std::tuple_size_v<T_tuple>) {
			parse_tuple_elements<T_language, T_tuple, T_index + 1>(current, tuple);
		}

	}

	template <typename T_language, typename T>
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

	template <typename T_language>
	std::string parse_string(token_iterator& current) {
		std::string_view token_value = current->value();

		bool error =
			(*(token_value.begin()) != '\"' || *(token_value.end() - 1) != '\"');

		if constexpr(T_language::allow_single_quoted_strings) {
			error &= (*(token_value.begin()) != '\'' || *(token_value.end() - 1) != '\'');
		} 

		if(error) {
			throw parse_error("String expected", current - 1);
		}

		return (current++)->copy_value().substr(1, token_value.size() - 2);
	}

	template <typename T_language>
	void parse_delimiter(token_iterator& current) {
		if constexpr(T_language::delimit_lists) {
			if((current++)->value() != ",") {
				throw parse_error("Expected ','", current - 1);
			}
		}
	}
}

#endif
