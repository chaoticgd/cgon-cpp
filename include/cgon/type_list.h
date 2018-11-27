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

#include <string>

#ifndef _CGON_TYPE_LIST_H
#define _CGON_TYPE_LIST_H

namespace cgon {
	struct type_list_end {};

	template <typename T_first, typename T_rest>
	struct type_list_element {
		using head = T_first;
		using tail = T_rest;
	};

	template <typename... T> struct make_type_list;

	template <typename T_first, typename ... T_rest>
	struct make_type_list<T_first, T_rest...> { 
		using type = type_list_element<T_first, typename make_type_list<T_rest...>::type>;
	};

	template <>
	struct make_type_list<> { using type = type_list_element<type_list_end, type_list_end>; };

	template <typename... T>
	using type_list = typename make_type_list<T...>::type;
}

#endif
