#include <iostream>
#include <cgon/document_schema.h>

class strings_example : public cgon::object {
public:

	std::string greeting() { return _greeting; }
	void set_greeting(std::string greeting) { _greeting = greeting; }

	std::string multi_line() { return _multi_line; }
	void set_multi_line(std::string multi_line) { _multi_line = multi_line; }

	using type_name = decltype("strings_example"_cgon_s);
	using child_types = std::tuple<>;
	using properties = cgon::type_list<
		cgon::property<decltype("greeting"_cgon_s), std::string, strings_example, &strings_example::greeting, &strings_example::set_greeting>,
		cgon::property<decltype("multi_line"_cgon_s), std::string, strings_example, &strings_example::multi_line, &strings_example::set_multi_line>
	>;

private:
	std::string _greeting;
	std::string _multi_line;
};

int main() {
	cgon::document_schema<strings_example> strings_schema;

	std::unique_ptr<strings_example> strings =
		strings_schema.read_file("strings.cgon");

	std::cout << strings->greeting() << std::endl;
	std::cout << strings->multi_line() << std::endl;
}
