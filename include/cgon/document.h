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

#ifndef _CGON_DOCUMENT_SCHEMA_H
#define _CGON_DOCUMENT_SCHEMA_H

#include <string>
#include <fstream>

#include "lexer.h"
#include "parser.h"
#include "unexpected_eof_error.h"
#include "languages.h"

namespace cgon {

	template <typename T, typename T_parser = cgon_parser>
	std::unique_ptr<T> read_file(std::string file_path);

	template <typename T, typename T_parser = cgon_parser>
	std::unique_ptr<T> read_string(std::string data);

	template <typename T, typename T_parser>
	std::unique_ptr<T> read_file(std::string file_path) {
		std::ifstream file(file_path);
		std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		return read_string<T, T_parser>(data);
	}

	template <typename T, typename T_parser>
	std::unique_ptr<T> read_string(std::string data) {
		
		std::vector<token> tokens = tokenize<typename T_parser::config>(data);

		token_iterator current(tokens.begin(), tokens.begin(), tokens.end());

		if constexpr(T_parser::config::require_explicit_top_level_type_name) {
			if((current++)->value() != get_string<typename T::type_name>::value()) {
				throw parse_error("Invalid root object type.", current - 1);
			}
		}

		std::unique_ptr<T> root;
		try {
			root.reset(T_parser::template parse_object_of_type<T>(current).release());
		} catch(unexpected_end_of_file_error e) {
			throw parse_error("Unexpected end of file", token_iterator(tokens.end() - 1, tokens.begin(), tokens.end()));
		}

		if(current != tokens.end()) {
			throw parse_error("Junk after root object", current);
		}

		return root;
	}

}

#endif
