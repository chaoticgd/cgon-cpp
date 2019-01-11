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

#ifndef _CGON_FUNCTION_EXPRESSION_H
#define _CGON_FUNCTION_EXPRESSION_H

#include <any>
#include <tuple>
#include <vector>
#include <functional>

#include "../token.h"
#include "../parse_error.h"
#include "../languages.h"

namespace cgon {

	template <typename T>
	struct function_traits;

	template <typename T_return_type, typename... T_arguments>
	struct function_traits<std::function<T_return_type(T_arguments...)>> {
		// Since the std::function member type is deprecated.
		using return_type = T_return_type;
		using first_argument = std::tuple_element_t<0, std::tuple<T_arguments...>>;
		using second_argument = std::tuple_element_t<1, std::tuple<T_arguments...>>;
	};

	template <typename T_name, typename T_function_type, T_function_type* T_function>
	struct function {
		using name = T_name;
		using traits = function_traits<std::function<T_function_type>>;
		
		static typename traits::return_type call(typename traits::first_argument arguments,
		                                         typename traits::second_argument data) {
			return T_function(arguments, data);
		}
	};

	template <typename T_parser, typename T_functions>
	struct function_expression_pre_parser
		: public T_parser::template base<function_expression_pre_parser<T_parser, T_functions>> {
		
		template <typename T_sub_type>
		using base = typename T_parser::template base<T_sub_type>;
		
		template <typename T>
		T parse_expression(token_iterator& current) {
			traverse_function_expression<T, 0>(current);
			return T();
		}

		template <typename T, std::size_t T_index>
		void traverse_function_expression(token_iterator& current) {

			using current_function = std::tuple_element_t<T_index, T_functions>;

			if(current->value() == get_string<typename current_function::name>::value()) {
				current++; // Skip over function name.
				using argument_type = typename current_function::traits::first_argument;
				parse_expression<argument_type>(current);
				return;
			}

			if constexpr(T_index + 1 < std::tuple_size_v<T_functions>) {
				traverse_function_expression<T, T_index + 1>(current);
				return;
			}

			T_parser
				::template base<function_expression_pre_parser<T_parser, T_functions>>
				::template parse_expression<T>(current);
		}
	};

	template <typename T_parser, typename T_functions, typename... T_data>
	struct function_expression_parser
		: public T_parser::template base<function_expression_parser<T_parser, T_functions, T_data...>> {
		
		template <typename T_sub_type>
		using base = typename T_parser::template base<T_sub_type>;
		
		function_expression_parser(T_data... data)
			: _data(data...) {}

		template <typename T>
		T parse_expression(token_iterator& current) {
			return parse_function_expression<T, 0>(current);
		}

		template <typename T, std::size_t T_index>
		T parse_function_expression(token_iterator& current) {
			using current_function = std::tuple_element_t<T_index, T_functions>;

			if(current->value() == get_string<typename current_function::name>::value()) {
				current++; // Skip over function name.
				using argument_type = typename current_function::traits::first_argument;
				argument_type arg = parse_expression<argument_type>(current);
				return current_function::call(arg, _data);
			}

			if constexpr(T_index + 1 < std::tuple_size_v<T_functions>) {
				parse_function_expression<T, T_index + 1>(current);
				return;
			}

			return T_parser
				::template base<function_expression_parser<T_parser, T_functions, T_data...>>
				::template parse_expression<T>(current);
		}
	
	private:
		std::tuple<T_data...> _data;
	};

	template <typename T_value, typename T_functions, typename... T_data>
	class function_expression {
	public:

		function_expression() {}

		template <typename T_parser>
		void parse(token_iterator& current) {
			
			token_iterator begin = current;
			function_expression_pre_parser<T_parser, T_functions> parser;
			parser.template parse_expression<T_value>(current);
			
			std::size_t text_size = current->offset() - begin->offset();
			_expression_text = begin->whole_text().substr(begin->offset(), text_size);

			std::size_t num_tokens = std::distance(begin, current);
			_tokens.reserve(num_tokens + 1);
			for(std::size_t i = 0; i < num_tokens; i++) {
				_tokens[i] = token(*(begin + i), _expression_text, begin->offset());
			}
			_tokens.back() = token("", "", 0); // Append an empty token to avoid an unexpected EOF error.
		}

		template <typename T_parser>
		T_value evaluate(T_data... data) {
			function_expression_pre_parser<T_parser, T_functions, T_data...> parser(data...);
			token_iterator current(_tokens.begin(), _tokens.begin(), _tokens.end());
			return parser.template parse_expression<T_value>(current);
		}

	private:
		std::string _expression_text;
		std::vector<token> _tokens;
	};
}

#endif
