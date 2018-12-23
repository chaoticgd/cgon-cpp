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

#ifndef _CGON_LEXER_H
#define _CGON_LEXER_H

#include <regex>
#include <tuple>
#include <string>
#include <vector>
#include <algorithm>
#include <string_view>
#include <cctype>

#include <boost/algorithm/string.hpp>

#include "token.h"

namespace cgon {

	static std::vector<token> tokenize(std::string_view text);
	static std::tuple<std::string_view, std::string_view> consume(std::string_view text);

	static bool is_symbol(char c);
	static std::string_view::iterator find_end_of_string(std::string_view text);

	std::vector<token> tokenize(std::string_view text) {
		std::string_view current = text;

		std::vector<token> tokens;

		while(current.size() > 0) {

			// Remove unnecessary whitespace.
			if(isspace(current[0])) {
				auto new_position = std::find_if_not(current.begin(), current.end(), isspace);
				current.remove_prefix(std::distance(current.begin(), new_position));
				continue;
			}

			const auto [ value, rest ] = consume(current);
			current = rest;

			tokens.emplace_back(value, text.size() - current.size(), text);
		}

		return tokens;
	}

	std::tuple<std::string_view, std::string_view> consume(std::string_view text) {

		char first = text[0];
		bool is_string = first == '\'' || first == '\"';
		
		std::string_view::iterator delimeter;
		if(is_string) {
			delimeter = find_end_of_string(text);
		} else if(is_symbol(first)) {
			delimeter = text.begin() + 1;
		} else {
			delimeter = std::find_if(text.begin(), text.end(), is_symbol);
		}

		size_t size = std::distance(text.begin(), delimeter);

		return { text.substr(0, size), text.substr(size) };
	}

	bool is_symbol(char c) {
		const static std::string symbols = "{}[]():\"',"; // Todo: Fix for JSON.
		return std::find(symbols.begin(), symbols.end(), c) != symbols.end() || isspace(c);
	}

	std::string_view::iterator find_end_of_string(std::string_view text) {
		for(auto iter = text.begin() + 1; iter < text.end(); iter++) {
			if(*iter == '\\') {
				iter++;
			} else if(*iter == *text.begin()) {
				return ++iter;
			}
		}

		return text.end();
	}
}

#endif
