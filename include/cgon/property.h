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

		template <typename T_language_f>
		friend struct parser;

		using name = T_name;
		using type = T_type;

	private:

		static T_type get(T_owner* owner) {
			return (owner->*T_getter)();
		}

		static void set(T_owner* owner, T_type& value) {
			(owner->*T_setter)(value);
		}

		const static bool is_property_list = false;
	};

	template <typename T_name, typename T_type, typename T_owner,
	          T_type T_owner::*T_pointer>
	struct pointer_property {

		template <typename T_language_f>
		friend struct parser;

		using name = T_name;
		using type = T_type;

	private:

		static T_type get(T_owner* owner) {
			return (owner->*T_pointer);
		}

		static void set(T_owner* owner, T_type& value) {
			(owner->*T_pointer) = std::move(value);
		}

		const static bool is_property_list = false;
	};

	template <typename T_name, typename T_type>
	struct property_list {

		template <typename T_language_f>
		friend struct parser;

		using name = T_name;
		using type = T_type;

	private:

		template <std::size_t T_index>
		constexpr static bool contains_optional() {

			if constexpr(is_optional<std::tuple_element_t<T_index, T_type>>::value) {
				return true;
			}

			if constexpr(T_index + 1 < std::tuple_size_v<T_type>) {
				return contains_optional<T_index + 1>();
			}

			return false;
		}

		const static bool is_property_list = true;
	};
}

#endif
