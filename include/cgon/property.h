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
#include "arithmetic_expression.h"
#include "string_expression.h"

namespace cgon {
	class object;

	class base_property {
		template <typename T>
		friend std::unique_ptr<object> parse_object_of_type(token_iterator& current, token_iterator end);
	public:
		virtual ~base_property() {}

		virtual std::string name() const { return ""; };
		virtual bool is_ready() const { return false; }

	protected:
		virtual void parse(token_iterator& current, token_iterator end) {}
	};

	template <typename T>
	class property : public base_property {
	public:
		virtual T get() const {}
		virtual void set(T value) {}

	protected:
		void parse(token_iterator& current, token_iterator end) {
			if constexpr(std::is_integral<T>() || std::is_floating_point<T>()) {
				arithmetic_expression expression(current, end);
				set(expression.value());
			} else if constexpr(std::is_same<T, std::string>()) {
				string_expression expression(current, end);
				set(expression.value());
			} else {
				throw std::runtime_error("not yet implemented");
			}
		}
	};
}

#endif
