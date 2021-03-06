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

#ifndef _CGON_PARSE_ERROR_H
#define _CGON_PARSE_ERROR_H

#include <stdexcept>
#include <string>

#include "token.h"

namespace cgon {
	class parse_error : public std::runtime_error {
	public:
		parse_error(const std::string& what)
			: std::runtime_error(what + "."),
			  _message(what) {}

		parse_error(const std::string& what, token_iterator where)
			: std::runtime_error(what),
			  _message(std::string(what) + ":\n\t" + where.get_line()) {}

		const char* what() const noexcept {
			return _message.c_str();
		}
	private:
		std::string _message;
	};
}

#endif
