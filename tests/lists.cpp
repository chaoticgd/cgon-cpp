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

struct pair : public cgon::object {
	std::string key;
	std::string value;

	using type_name = decltype("pair"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::pointer_property<decltype("key"_cgon_s), std::string, pair, &pair::key>,
		cgon::pointer_property<decltype("value"_cgon_s), std::string, pair, &pair::value>
	>;
};

struct lists_of_objects : public cgon::object {
	std::vector<std::unique_ptr<pair>> pairs;

	using type_name = decltype("lists_of_objects"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::pointer_property<decltype("pairs"_cgon_s), std::vector<std::unique_ptr<pair>>,
		                       lists_of_objects, &lists_of_objects::pairs>
	>;
};

TEST(lists, of_object) {
	auto root = cgon::read_file<lists_of_objects>("lists_of_objects.cgon");

	EXPECT_EQ(root->pairs[0]->key, "a");
	EXPECT_EQ(root->pairs[0]->value, "b");
	EXPECT_EQ(root->pairs[1]->key, "c");
	EXPECT_EQ(root->pairs[1]->value, "d");
	EXPECT_EQ(root->pairs[2]->key, "e");
	EXPECT_EQ(root->pairs[2]->value, "f");
}
