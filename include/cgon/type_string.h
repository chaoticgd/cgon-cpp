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

#ifndef _CGON_TYPE_STRING_H
#define _CGON_TYPE_STRING_H

#include <utility>

namespace cgon {
	template <char... T_characters>
	using type_string = std::integer_sequence<char, T_characters...>;

	template <typename>
	struct get_string;

	template <char... T_elements>
	struct get_string<type_string<T_elements...>> {
		static const char* value() {
			constexpr static const char result[sizeof...(T_elements) + 1] = { T_elements..., '\0' };
			return result;
		}
	};
}

template <typename T, T... T_characters>
constexpr cgon::type_string<T_characters...> operator""_cgon_s() { return{}; }

#endif
