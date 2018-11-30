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
#include <cgon/value_property.h>

class c : public cgon::object {
public:
	c() : _d(this, "d") {}

	static std::string type_name() { return "c"; }
	using child_types = cgon::type_list<>;

	cgon::value_property<double> _d;
};

class a : public cgon::object {
public:
	a() : _b(this, "b") {}

	static std::string type_name() { return "a"; }
	using child_types = cgon::type_list<c>;

	cgon::value_property<double> _b;
};

TEST(minified, main) {
	cgon::document_schema<a> minified_schema;

	std::unique_ptr<a> root =
		minified_schema.read_file("minified/minified.cgon");

	EXPECT_EQ(root->_b.get(), 1337);

	c* child = root->children_of_type<c>()[0];	
	EXPECT_EQ(child->_d.get(), 42);
}
