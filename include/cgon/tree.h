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
		tree_node() : _parent(nullptr) {}

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
			T* child = new T_child(args...);
			child->_parent = static_cast<T*>(this);
			_children.emplace_back(child);
			return child;
		}

		T* append_child(std::unique_ptr<T>& child) {
			child->_parent = static_cast<T*>(this);
			_children.emplace_back(std::move(child));
			return (_children.end() - 1)->get();
		}

		T* insert_child(std::unique_ptr<T>& child, size_t position) {
			child->_parent = static_cast<T*>(this);
			T* raw = child.get();
			_children.insert(_children.begin() + position, std::move(child));
			return raw;
		}

		std::unique_ptr<T> remove_child(T* child) {
			std::unique_ptr<T> removedChild(nullptr);
			auto iter = std::remove_if(
				_children.begin(), _children.end(),
				[=, &removedChild](std::unique_ptr<T>& current) {
					bool removeCurrent = current.get() == child;
					if(removeCurrent) {
						removedChild.reset(current.release());
					}
					return removeCurrent;
				});
			_children.erase(iter, _children.end());

			return std::move(removedChild);
		}

		T* parent() {
			return _parent;
		}

	private:
		std::vector<std::unique_ptr<T>> _children;
		T* _parent;
	};
}

#endif
