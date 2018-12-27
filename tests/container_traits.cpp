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

#include <cgon/container_traits.h>

TEST(container_traits, is_optional) {
	EXPECT_TRUE(cgon::is_optional<std::optional<int>>::value);
	EXPECT_TRUE(cgon::is_optional<std::optional<std::optional<int>>>::value);

	EXPECT_FALSE(cgon::is_optional<int>::value);
	EXPECT_FALSE(cgon::is_optional<std::unique_ptr<std::optional<int>>>::value);
}

TEST(container_traits, is_vector) {
	EXPECT_TRUE(cgon::is_vector<std::vector<int>>::value);

	using vector_type = std::vector<int, int>;
	EXPECT_TRUE(cgon::is_vector<vector_type>::value);

	EXPECT_FALSE(cgon::is_vector<int>::value);
}

TEST(container_traits, is_array) {
	using array_type = std::array<int, 2>;
	EXPECT_TRUE(cgon::is_array<array_type>::value);

	EXPECT_FALSE(cgon::is_array<int>::value);
}

TEST(container_traits, tuple) {
	using null_tuple = std::tuple<>;
	EXPECT_TRUE(cgon::is_tuple<null_tuple>::value);

	using single = std::tuple<int>;
	EXPECT_TRUE(cgon::is_tuple<single>::value);

	using pair = std::tuple<int, int>;
	EXPECT_TRUE(cgon::is_tuple<pair>::value);

	EXPECT_FALSE(cgon::is_tuple<int>::value);
}

TEST(container_traits, unique_ptr) {
	EXPECT_TRUE(cgon::is_unique_ptr<std::unique_ptr<int>>::value);

	using unique_ptr_type = std::unique_ptr<int, int>;
	EXPECT_TRUE(cgon::is_unique_ptr<unique_ptr_type>::value);

	EXPECT_FALSE(cgon::is_unique_ptr<int>::value);
}
