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

#include <cgon/document_schema.h>

class numbers : public cgon::object {
public:

	int a() { return _a; }
	void set_a(int a) { _a = a; }

	int b() { return _b; }
	void set_b(int b) { _b = b; }

	int c() { return _c; }
	void set_c(int c) { _c = c; }

	double d() { return _d; }
	void set_d(double d) { _d = d; }

	using type_name = decltype("numbers"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::property<decltype("a"_cgon_s), int, numbers, &numbers::a, &numbers::set_a>,
		cgon::property<decltype("b"_cgon_s), int, numbers, &numbers::b, &numbers::set_b>,
		cgon::property<decltype("c"_cgon_s), int, numbers, &numbers::c, &numbers::set_c>,
		cgon::property<decltype("d"_cgon_s), double, numbers, &numbers::d, &numbers::set_d>
	>;

private:
	int _a, _b, _c;
	double _d;
};

struct booleans : public cgon::object {
	bool a, b;

	using type_name = decltype("booleans"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::pointer_property<decltype("a"_cgon_s), bool, booleans, &booleans::a>,
		cgon::pointer_property<decltype("b"_cgon_s), bool, booleans, &booleans::b>
	>;
};

TEST(primitives, numbers) {
	cgon::document_schema<numbers> schema;
	std::unique_ptr<numbers> root =
		schema.read_file("primitives/numbers.cgon");

	EXPECT_EQ(root->a(), 1337);
	EXPECT_EQ(root->b(), 123456789);
	EXPECT_EQ(root->c(), -32);
	EXPECT_DOUBLE_EQ(root->d(), 42.42);
}

TEST(primitives, booleans) {
	cgon::document_schema<booleans> schema;
	std::unique_ptr<booleans> root =
		schema.read_file("primitives/booleans.cgon");
	
	EXPECT_EQ(root->a, false);
	EXPECT_EQ(root->b, true);
}
