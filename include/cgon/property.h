#ifndef _CGON_PROPERTY_H
#define _CGON_PROPERTY_H

#include <type_traits>

#include "type_registrar.h"

namespace cgon {
	class base_object;

	class base_property {
	public:
		virtual ~base_property() {}

		virtual std::string name() const { return ""; };
		virtual bool is_ready() const { return false; }

		// TODO: Clean this up.
		virtual bool is_integral() { return false; }
		virtual bool is_floating_point() { return false; }
		virtual bool is_string() { return false; }
		virtual type_registrar<base_object> get_type() { return type_registrar<base_object>(); };
	};

	template <typename T>
	class property : public base_property {
	public:
		bool is_integral() {
			return std::is_integral<T>();
		}

		bool is_floating_point() {
			return std::is_floating_point<T>();
		}

		bool is_string() {
			return std::is_same<T, std::string>();
		}

		virtual T get() const {}
		virtual void set(T value) {}
	};
}

#endif
