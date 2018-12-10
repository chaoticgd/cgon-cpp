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
	static std::tuple<std::string, std::string_view> consume(std::string_view text);

	static bool is_symbol(char c);
	static int find_indent_level(std::string_view text);
	static std::string_view::iterator find_end_of_string(std::string_view text);
	static std::string unindent_string(std::string text, int indent_level);
	static std::string unescape_string(std::string token);

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

	std::tuple<std::string, std::string_view> consume(std::string_view text) {

		char first = text[0];
		bool is_string = first == '\'' || first == '\"';
		
		std::string_view::iterator delimeter;
		if(is_string) {
			int indent_level = find_indent_level(text);
			delimeter = find_end_of_string(text);

			size_t size = std::distance(text.begin(), delimeter);
			std::string content = static_cast<std::string>(text.substr(1, size - 2));
			if(first == '\"') {
				boost::trim(content);
				content = unindent_string(content, indent_level + 1);
			}
			return { std::string("\'") + content + "\'", text.substr(size) };
		} else if(is_symbol(first)) {
			delimeter = text.begin() + 1;
		} else {
			delimeter = std::find_if(text.begin(), text.end(), is_symbol);
		}

		size_t size = std::distance(text.begin(), delimeter);

		std::string token = static_cast<std::string>(text.substr(0, size));
		if(is_string) {
			token = unescape_string(token);
		}
		return { token, text.substr(size) };
	}

	bool is_symbol(char c) {
		const static std::string symbols = "{}[]():\"'@";
		return std::find(symbols.begin(), symbols.end(), c) != symbols.end() || isspace(c);
	}

	int find_indent_level(std::string_view text) {
		std::string_view::iterator iter, last_token_pos;
		for(iter = text.begin() - 1; *iter != '\n'; iter--) {
			if(!isspace(*iter)) {
				last_token_pos = iter;
			}
		}
		return std::distance(iter, last_token_pos) - 1;
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

	std::string unindent_string(std::string text, int indent_level) {
		std::string result;

		std::vector<std::string> lines;
		boost::split(lines, text, boost::is_any_of("\n"));
		for(std::string& line : lines) {
			int characters_to_remove = 0;
			for(int i = 0; i < indent_level; i++) {
				if(line[i] == '\t') {
					characters_to_remove++;
				} else {
					break;
				}
			}
			result += line.substr(characters_to_remove) + '\n';
		}

		return result.substr(0, result.size() - 1); // Remove the last '\n'.
	}

	std::string unescape_string(std::string token) {
		return token; // TODO: Handle escaped characters.
	}
}

#endif
