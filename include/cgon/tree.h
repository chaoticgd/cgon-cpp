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

#ifndef _CGON_TREE_H
#define _CGON_TREE_H

#include <memory>
#include <vector>
#include <algorithm>

namespace cgon {
	template <typename T>
	class tree_node
	{
	protected:
		tree_node() {}
		virtual ~tree_node() {}

	public:

		std::vector<T*> children() const {
			std::vector<T*> result(_children.size());
			std::transform(
				_children.begin(), _children.end(), result.begin(),
				[](const std::unique_ptr<T>& child) { return child.get(); });
			return result;
		}

		template <typename T_child_type>
		std::vector<T_child_type*> children_of_type() const {
			std::vector<T_child_type*> result;
			for(const std::unique_ptr<T>& base_child : _children) {
				T_child_type* child = dynamic_cast<T_child_type*>(base_child.get());
				if(child != nullptr) {
					result.push_back(child);
				}
			}
			return result;
		}

		template <typename T_child, typename... T_constructor_args>
		T* create_child(T_constructor_args... args) {
			auto child = std::make_unique<T_child>(args...);
			return append_child(child);
		}

		T* append_child(std::unique_ptr<T>& child) {
			return insert_child(child, _children.size());
		}

		T* insert_child(std::unique_ptr<T>& child, size_t position) {
			T* raw = child.get();
			_children.insert(_children.begin() + position, std::move(child));
			return raw;
		}

		std::unique_ptr<T> remove_child(T* child) {
			auto iter = std::find_if(_children.begin(), _children.end(),
				[=](std::unique_ptr<T>& current) { return current.get() == child; });
			std::unique_ptr<T> orphaned_child = std::move(*iter);
			_children.erase(iter);
			return orphaned_child;
		}

	private:
		std::vector<std::unique_ptr<T>> _children;
	};
}

#endif
