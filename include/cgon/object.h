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

#ifndef _CGON_OBJECT_H
#define _CGON_OBJECT_H

#include <stdexcept>

#include "tree.h"
#include "property.h"

namespace cgon {
	class object : public tree_node<object> {
		
		template <typename T_sub_type_f, typename T_language_f>
		friend struct parser;

	public:
		template <typename T_child>
		T_child* find_child_of_type(std::string name) {
			for(T_child* child : children_of_type<T_child>()) {
				if(child->get_name() == name) {
					return child;
				}
			}

			throw std::out_of_range(std::string("No child of the correct type exists with name '") + name + "'");
		}

		std::string get_name() const {
			return _name;
		}

	protected:
		void set_name(std::string name) {
			_name = name;
		}

	private:
		std::string _name;
	};
}

#endif
