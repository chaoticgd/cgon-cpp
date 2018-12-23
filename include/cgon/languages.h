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
}

#endif
