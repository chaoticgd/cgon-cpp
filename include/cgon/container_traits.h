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

#ifndef _CGON_CONTAINER_TRAITS_H
#define _CGON_CONTAINER_TRAITS_H

#include <type_traits>
#include <optional>
#include <vector>
#include <array>
#include <tuple>
#include <memory>

namespace cgon {

	template <typename T> struct is_optional : std::false_type {};
	template <typename T> struct is_optional<std::optional<T>> : std::true_type {};

	template <typename T> struct is_vector : std::false_type {};
	template <typename T, typename T_allocator> struct is_vector<std::vector<T, T_allocator>> : std::true_type {};

	template <typename T> struct is_array : std::false_type {};
	template <typename T, std::size_t T_size> struct is_array<std::array<T, T_size>> : std::true_type {};

	template <typename T> struct is_tuple : std::false_type {};
	template <typename... T> struct is_tuple<std::tuple<T...>> : std::true_type {};

	template <typename T> struct is_unique_ptr : std::false_type {};
	template <typename T, typename T_deleter> struct is_unique_ptr<std::unique_ptr<T, T_deleter>> : std::true_type {};
	
}

#endif
