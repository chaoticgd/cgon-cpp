#include <iostream>
#include <cgon/document.h>

class strings_example : public cgon::object {
public:

	std::string greeting() { return _greeting; }
	void set_greeting(std::string greeting) { _greeting = greeting; }


	using type_name = decltype("strings_example"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::property<decltype("greeting"_cgon_s), std::string, strings_example, &strings_example::greeting, &strings_example::set_greeting>
	>;

private:
	std::string _greeting;
	std::string _multi_line;
};

int main() {
	std::unique_ptr<strings_example> strings =
		cgon::read_file<strings_example>("strings.cgon");

	std::cout << strings->greeting() << std::endl;
}
