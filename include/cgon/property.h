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

#ifndef _CGON_PROPERTY_H
#define _CGON_PROPERTY_H

#include <type_traits>

#include "token.h"
#include "container_traits.h"

namespace cgon {
	class object;

	template <typename T_name, typename T_type, typename T_owner,
	          T_type(T_owner::*T_getter)(), void(T_owner::*T_setter)(T_type value)>
	struct property {

		template <typename T_language, typename T_owner_func, typename T_properties, int T_index>
		friend void parse_property(token_iterator& current, T_owner_func* owner);

		using name = T_name;
		using type = T_type;

	private:

		static T_type get(T_owner* owner) {
			return (owner->*T_getter)();
		}

		static void set(T_owner* owner, T_type& value) {
			(owner->*T_setter)(value);
		}
	};

	template <typename T_name, typename T_type, typename T_owner,
	          T_type T_owner::*T_pointer>
	struct pointer_property {

		template <typename T_language, typename T_owner_func, typename T_properties, int T_index>
		friend void parse_property(token_iterator& current, T_owner_func* owner);

		using name = T_name;
		using type = T_type;

	private:

		static T_type get(T_owner* owner) {
			return (owner->*T_pointer);
		}

		static void set(T_owner* owner, T_type& value) {
			if constexpr(is_unique_ptr<T_type>::value) {
				(owner->*T_pointer).swap(value);
			} else {
				(owner->*T_pointer) = value;
			}
		}
	};
}

#endif
