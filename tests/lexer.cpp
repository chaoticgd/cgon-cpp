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

#include "gtest/gtest.h"

#include <cgon/document.h>

std::vector<std::string> extract_token_values(std::vector<cgon::token> tokens) {
	std::vector<std::string> result(tokens.size());
	std::transform(tokens.begin(), tokens.end(), result.begin(),
		[](cgon::token& t) { return t.copy_value(); });
	return result;
}

TEST(lexer, main) {
	std::string text = "lorem{ ip: \"sum\" test {one: 2 three: 4} }";
	std::vector<cgon::token> tokens = cgon::tokenize<cgon::language::cgon>(text);
	std::vector<std::string> token_values = extract_token_values(tokens);
	std::vector<std::string> expected_values = { "lorem", "{", "ip", ":", "\"sum\"", "test", "{", "one", ":", "2", "three", ":", "4", "}", "}" };
	EXPECT_EQ(token_values, expected_values);
}
