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
#include <cgon/value_property.h>

class numbers : public cgon::object {
public:
	numbers() : a(this, "a"), b(this, "b"), c(this, "c"), d(this, "d") {}

	static std::string type_name() { return "numbers"; }

	cgon::value_property<int> a, b, c;
	cgon::value_property<double> d;
};

TEST(numbers, main) {
	cgon::document_schema<numbers> colours_schema;

	std::unique_ptr<numbers> root =
		colours_schema.read_file("numbers/numbers.cgon");

	EXPECT_EQ(root->a.get(), 1337);
	EXPECT_EQ(root->b.get(), 123456789);
	EXPECT_EQ(root->c.get(), -32);
	EXPECT_DOUBLE_EQ(root->d.get(), 42.42);
}
