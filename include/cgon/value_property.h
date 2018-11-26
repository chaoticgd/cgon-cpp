#ifndef _CGON_VALUE_PROPERTY_H
#define _CGON_VALUE_PROPERTY_H

#include <string>
#include <type_traits>

#include "property.h"
#include "object.h"

namespace cgon {
	template <typename T>
	class value_property : public property<T> {
	public:
		value_property(base_object* owner, std::string name)
			: _name(name), _ready(false) {
			owner->register_property(name, this);
		}

		virtual std::string name() const {
			return _name;
		}

		bool is_ready() const {
			return _ready;
		}

		T get() const {
			return _value;
		}

		void set(T value) {
			_ready = true;
			_value = value;
		}

		type_registrar<base_object> get_type() {
			type_registrar<base_object> result;
			if constexpr (std::is_object<T>()) {
				//result.add<T>();
			}
			return result;
		}

	private:
		std::string _name;
		bool _ready;
		T _value;
	};
}

#endif