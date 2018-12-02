#include <iostream>
#include <cgon/document_schema.h>
#include <cgon/value_property.h>
#include <cgon/type_list.h>

class strings_example : public cgon::object {
public:
	strings_example() : _greeting(this, "greeting"), _multi_line(this, "multi_line") {}

	static std::string type_name() { return "strings_example"; }
	using child_types = cgon::type_list<>;

	std::string get_greeting() { return _greeting.get(); }
	std::string get_multi_line() { return _multi_line.get(); }

private:
	cgon::value_property<std::string> _greeting;
	cgon::value_property<std::string> _multi_line;
};

int main() {
	cgon::document_schema<strings_example> strings_schema;

	std::unique_ptr<strings_example> strings =
		strings_schema.read_file("strings.cgon");

	std::cout << strings->get_greeting() << std::endl;
	std::cout << strings->get_multi_line() << std::endl;
}
