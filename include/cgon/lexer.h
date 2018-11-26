#ifndef _CGON_LEXER_H
#define _CGON_LEXER_H

#include <regex>
#include <tuple>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <string_view>

#include "token.h"

namespace cgon {
	std::tuple<std::string, std::string_view> consume(std::string_view text) {
		auto delimeter = std::find_if(text.begin(), text.end(), isspace);
		size_t size = std::distance(text.begin(), delimeter);
		return { static_cast<std::string>(text.substr(0, size)), text.substr(size) };
	}

	std::vector<token> tokenize(std::string_view text) {
		std::string_view current = text;

		std::vector<token> tokens;

		const std::regex trivial("[^[ \t\n]]*");

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
}

#endif