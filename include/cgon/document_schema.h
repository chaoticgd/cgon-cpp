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

namespace cgon {
	template <typename T>
	class document_schema {
	public:
		document_schema() {}

		std::unique_ptr<T> read_file(std::string file_path) const {
			std::ifstream file(file_path);
			std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			return read_string(data);
		}

		std::unique_ptr<T> read_string(std::string data) const {
			
			std::vector<token> tokens = tokenize(data);
			
			token_iterator current = tokens.begin();

			base_object* generic_root = parse_object_of_type<T>(current, tokens.end()).release();
			std::unique_ptr<T> root(dynamic_cast<T*>(generic_root));

			if(current != tokens.end()) {
				throw parse_error("Junk after root object", current);
			}

			return root;
		}
	};
}

#endif
