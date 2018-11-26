#ifndef _CGON_DOCUMENT_
#define _CGON_DOCUMENT_

#include <string>
#include <fstream>

#include "lexer.h"
#include "parser.h"

namespace cgon {
	template <typename T>
	class document_schema {
	public:
		document_schema() {}

		std::unique_ptr<T> read_file(std::string file_path) const {
			std::ifstream file(file_path);
			std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			return read_string(data);
		}

		std::unique_ptr<T> read_string(std::string data) const {
			
			std::vector<token> tokens = tokenize(data);
			
			token_iterator current = tokens.begin();

			type_registrar<base_object> allowed_root_type;
			allowed_root_type.add<T>(T::type_name());

			base_object* generic_root = parse_object(current, tokens.end(), allowed_root_type).release();
			std::unique_ptr<T> root(dynamic_cast<T*>(generic_root));

			if(current != tokens.end()) {
				throw parse_error("Junk after root object", current);
			}

			return root;
		}
	};
}

#endif