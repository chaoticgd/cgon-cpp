#ifndef _CGON_TYPE_STRING_H
#define _CGON_TYPE_STRING_H

#include <utility>

namespace cgon {
	template <char... T_characters>
	using type_string = std::integer_sequence<char, T_characters...>;

	template <typename>
	struct get_string;

	template <char... T_elements>
	struct get_string<type_string<T_elements...>> {
		static const char* value() {
			constexpr static const char result[sizeof...(T_elements) + 1] = { T_elements..., '\0' };
			return result;
		}
	};
}

template <typename T, T... T_characters>
	constexpr cgon::type_string<T_characters...> operator""_cgon_s() { return{}; }

#endif
