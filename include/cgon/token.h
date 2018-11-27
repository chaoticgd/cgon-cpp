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

#ifndef _CGON_TOKEN_H
#define _CGON_TOKEN_H

#include <string>
#include <string_view>

namespace cgon {
	class token {
	public:
		token(std::string value, size_t offset, std::string_view text)
			: _value(value), _line(1), _column(1) {
			
			for(int i = 0; i < offset - value.size(); i++) {
				if(text[i] == '\n') {
					_line++;
					_column = 1;
				} else {
					_column++;
				}
			}
			
		}

		std::string value() const { return _value; }
		int line() const { return _line; }

		std::string to_string() const {
			std::string position = std::to_string(_line) + ":" + std::to_string(_column);
			return std::string("'") + value() + "' at " + position;
		}

	private:
		std::string _value;
		size_t _line, _column;
	};

	typedef std::vector<token>::iterator token_iterator;

	std::string token_get_line(token_iterator iter) {
		token_iterator lower = iter, upper = iter;
		while(lower->line() == iter->line()) {
			lower--;
		}
		while(upper->line() == iter->line()) {
			upper++;
		}
			
		std::string result;

		for(token_iterator current = lower; current < upper; current++) {
			if(current == iter) {
				result += "\x1B[31m"; // Set the text colour to red.
			}
			result += current->value() + " ";
			if(current == iter) {
				result += "\033[0m"; // Restore the text colour.
			}
		}
		return result;
	} 
}

#endif
