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

#include <gtest/gtest.h>

#include <cgon/document.h>
#include <cgon/extras/function_expression.h>

int add(std::tuple<int, int> arguments, std::tuple<> data) {
	return std::get<0>(arguments) + std::get<1>(arguments);
}

int subtract(std::tuple<int, int> arguments, std::tuple<> data) {
	return std::get<0>(arguments) - std::get<1>(arguments);
}

struct function_expression_test : public cgon::object {

	using arithmetic_expression = cgon::function_expression<int, std::tuple<
		cgon::function<decltype("add"_cgon_s), decltype(add), add>,
		cgon::function<decltype("subtract"_cgon_s), decltype(subtract), subtract>
	>>;
	arithmetic_expression value1;
	arithmetic_expression value2;

	using type_name = decltype("function_expression_test"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::pointer_property<decltype("value1"_cgon_s), decltype(value1),
		                       function_expression_test, &function_expression_test::value1>,
		cgon::pointer_property<decltype("value2"_cgon_s), decltype(value2),
		                       function_expression_test, &function_expression_test::value2>
	>;
};

TEST(function_expression_test, function_expression_test) {
	std::unique_ptr<function_expression_test> root =
		cgon::read_file<function_expression_test, cgon::cgon_parser>
		("function_expression_test.cgon");
	
	EXPECT_EQ(root->value1.evaluate<cgon::cgon_parser>(), 42);
	EXPECT_EQ(root->value2.evaluate<cgon::cgon_parser>(), 1337);
}
