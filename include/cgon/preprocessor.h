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

#ifndef _CGON_PREPROCESSOR_H
#define _CGON_PREPROCESSOR_H

#include <string>
#include <vector>
#include <tuple>

#include "parse_error.h"

namespace cgon {
	static std::string strip_comments(std::string text);
	static bool find_next_comment(std::string& text, std::size_t& position, const char* pattern);

	std::string strip_comments(std::string text) {
		
		const std::vector<std::tuple<const char*, const char*, int>> comment_types {
			{ "/*", "*/", 2 },
			{ "//", "\n", 0 }
		};

		for(auto comment_type : comment_types) {
			std::size_t position = -1;
			while(find_next_comment(text, position, std::get<0>(comment_type))) {
				std::size_t end_position = text.find(std::get<1>(comment_type), position);
				if(end_position == std::string::npos) {
					throw parse_error("Unterminated comment");
				}
				text = text.substr(0, position) + text.substr(end_position + std::get<2>(comment_type));
			}
		}
		
		return text;
	}

	bool find_next_comment(std::string& text, std::size_t& position, const char* pattern) {
		position = text.find(pattern);
		return position != std::string::npos;
	}
}

#endif
