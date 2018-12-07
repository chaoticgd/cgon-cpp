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

class c : public cgon::object {
public:

	int d() { return _d; }
	void set_d(int d) { _d = d; }

	using type_name = decltype("c"_cgon_s);
	using child_types = cgon::type_list<>;
	using properties = cgon::type_list<
		cgon::property<decltype("d"_cgon_s), int, c, &c::d, &c::set_d>
	>;

private:
	int _d;
};

class a : public cgon::object {
public:

	int b() { return _b; }
	void set_b(int b) { _b = b; }

	using type_name = decltype("a"_cgon_s);
	using child_types = cgon::type_list<c>;
	using properties = cgon::type_list<
		cgon::property<decltype("b"_cgon_s), int, a, &a::b, &a::set_b>
	>;

private:
	int _b;
};

TEST(minified, main) {
	cgon::document_schema<a> minified_schema;

	std::unique_ptr<a> root =
		minified_schema.read_file("minified/minified.cgon");

	EXPECT_EQ(root->b(), 1337);

	c* child = root->children_of_type<c>()[0];	
	EXPECT_EQ(child->d(), 42);
}
