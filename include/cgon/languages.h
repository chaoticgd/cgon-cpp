#ifndef _CGON_LANGUAGES_H
#define _CGON_LANGUAGES_H

#include "type_string.h"

namespace cgon {
	namespace language {
		struct cgon {
			static const bool require_explicit_top_level_type_name = true;
			static const bool allow_children = true;
			static const bool allow_tuples = true;
			static const bool allow_comments = true;
			static const bool allow_named_objects = true;
			static const bool allow_single_quoted_strings = true;

			static const bool quoted_property_names = false;

			static const bool delimit_lists = false;
		};

		struct json {
			static const bool require_explicit_top_level_type_name = false;
			static const bool allow_children = false;
			static const bool allow_tuples = false;
			static const bool allow_comments = false;
			static const bool allow_named_objects = false;
			static const bool allow_single_quoted_strings = false;

			static const bool quoted_property_names = true;

			static const bool delimit_lists = true;
		};
	}

	template <typename T_sub_type>
	struct cgon_parser_base : public parser<T_sub_type, language::cgon> {};

	template <typename T_sub_type>
	struct json_parser_base : public parser<T_sub_type, language::json> {};

	struct cgon_parser : public cgon_parser_base<cgon_parser> {
		template <typename T_sub_type>
		using base = cgon_parser_base<T_sub_type>;
	};

	struct json_parser : public json_parser_base<json_parser> {
		template <typename T_sub_type>
		using base = json_parser_base<T_sub_type>;
	};
}

#endif
