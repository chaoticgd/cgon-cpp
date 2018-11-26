#ifndef _CGON_OBJECT_H
#define _CGON_OBJECT_H

#include <stdexcept>

#include "tree.h"
#include "type_registrar.h"
#include "property.h"

namespace cgon {
	class base_object : public tree_node<base_object> {
		friend std::unique_ptr<base_object> parse_object(token_iterator& current, token_iterator end, type_registrar<base_object> allowed_child_types);
	public:

		static std::string type_name();

		type_registrar<base_object> allowed_child_types() const {
			return _allowed_child_types;
		}

		void register_property(std::string name, base_property* property) {
			_properties.emplace(name, property);
		}

	protected:

		std::string get_name() const {
			return _name;
		}

		void set_name(std::string name) {
			_name = name;
		}

		template <typename T>
		void add_allowed_child_type(std::string name) {
			_allowed_child_types.add<T>(name);
		}

		bool has_property(std::string name) {
			return _properties.find(name) != _properties.end();
		}

		template <typename T>
		bool has_property_of_type(std::string name) {
			return _properties.find(name) != _properties.end() &&
			       dynamic_cast<T*>(_properties.at(name)) != nullptr;
		}

		base_property* get_property(std::string name) {
			return _properties.at(name);
		}

		template <typename T>
		property<T>* get_property_of_type(std::string name) {
			base_property* result = _properties.at(name);
			return dynamic_cast<property<T>*>(result);
		}

		template <typename T>
		void set_property(std::string name, T value) {
			get_property_of_type<T>(name)->set(value);
		}

		std::vector<base_property*> properties() {
			std::vector<base_property*> result(_properties.size());
			std::transform(_properties.begin(), _properties.end(), result.begin(),
				[](const std::pair<std::string, base_property*>& elem) { return elem.second; });
			return result;
		}

	private:
		std::string _name;
		std::map<std::string, base_property*> _properties;
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