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

			static const bool quote_property_names = false;

			using list_delimiter = decltype(""_cgon_s);
		};
	}
}

#endif
