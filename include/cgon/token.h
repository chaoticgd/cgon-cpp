#ifndef _CGON_TOKEN_H
#define _CGON_TOKEN_H

#include <string>
#include <string_view>

namespace cgon {
	class token {
	public:
		token(std::string value, size_t offset, std::string_view text)
			: _value(value), _line(1), _column(1) {
			
			for(int i = 0; i < offset - value.size(); i++) {
				if(text[i] == '\n') {
					_line++;
					_column = 1;
				} else {
					_column++;
				}
			}
			
		}

		std::string value() const { return _value; }
		size_t line() const { return _line; }
		size_t column() const { return _column; }

		std::string to_string() const {
			return std::string("'") + value() + "' at " + std::to_string(line()) + ":" + std::to_string(column());
		}

	private:
		std::string _value;
		size_t _line, _column;
	};

	typedef std::vector<token>::iterator token_iterator;
}

#endif