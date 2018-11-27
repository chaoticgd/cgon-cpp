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

#ifndef _CGON_TYPE_REGISTRAR_H
#define _CGON_TYPE_REGISTRAR_H

#include <functional>
#include <vector>
#include <map>

namespace cgon {
	template <typename T>
	struct type {
		std::function<std::unique_ptr<T>()> create;
	};

	template <typename T>
	class type_registrar {
	public:
		type_registrar() {}

		bool contains(std::string name) const {
			return _types.find(name) != _types.end();
		}

		type<T> get(std::string name) const {
			return _types.at(name);
		}

		template <typename T_type_to_register>
		void add(std::string type_name) {
			type<T> value { std::make_unique<T_type_to_register> };
			_types.emplace(type_name, value);
		}

	private:
		std::map<std::string, type<T>> _types;
	};
}
#endif
