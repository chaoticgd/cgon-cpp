#ifndef _CGON_OBJECT_H
#define _CGON_OBJECT_H

#include "tree.h"
#include "type_registrar.h"

namespace cgon {
	class base_object : public tree_node<base_object> {
	public:

		static std::string type_name();

		type_registrar<base_object> allowed_child_types() const {
			return _allowed_child_types;
		}

	protected:
		template <typename T>
		void add_allowed_child_type(std::string name) {
			_allowed_child_types.add<T>(name);
		}

	private:
		type_registrar<base_object> _allowed_child_types;
	};

	class object : public virtual base_object {};
	
	template <typename T_child_type>
	class allow_children_of_type : public virtual base_object {
	public:
		allow_children_of_type() {
			add_allowed_child_type<T_child_type>(T_child_type::type_name());
		}
	};
}

#endif