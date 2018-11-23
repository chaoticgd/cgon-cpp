#include <iostream>
#include <cgon/document.h>

class colour : public cgon::object {
public:
	colour() {}

	static std::string type_name() { return "colour"; }
};

class colour_list
	: public cgon::object,
	  cgon::allow_children_of_type<colour> {
public:
	colour_list() {

	}

	static std::string type_name() { return "colour_list"; }

private:
	// cgon::value_property<int> r, g, b;
};

int main() {
	cgon::document_schema<colour_list> colours_schema;
	std::unique_ptr<colour_list> colours = colours_schema.read_file("colours.cgon");

	for(colour* c : colours->children_of_type<colour>()) {
		std::cout << "Colour" << std::endl;
	}
}