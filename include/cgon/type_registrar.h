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
