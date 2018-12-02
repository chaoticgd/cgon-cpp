#include <iostream>
#include <cgon/document_schema.h>
#include <cgon/value_property.h>
#include <cgon/type_list.h>

class colour : public cgon::object {
public:
	colour() : _r(this, "r"), _g(this, "g"), _b(this, "b") {}

	static std::string type_name() { return "colour"; }
	using child_types = cgon::type_list<>;

	int red() { return _r.get(); }
	int green() { return _g.get(); }
	int blue() { return _b.get(); }

	std::string name() { return get_name(); }

private:
	cgon::value_property<int> _r, _g, _b;
};

class colour_list
	: public cgon::object {
public:
	colour_list() {
	}

	static std::string type_name() { return "colour_list"; }
	using child_types = cgon::type_list<colour>;
};

int main() {
	cgon::document_schema<colour_list> colours_schema;
	
	std::unique_ptr<colour_list> colours =
		colours_schema.read_file("colours.cgon");

	for(colour* c : colours->children_of_type<colour>()) {
		std::cout << c->name() << ": " << c->red() << " " << c->green() << " " << c->blue() << std::endl;
	}
}