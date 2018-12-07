#include <iostream>
#include <cgon/document_schema.h>

class colour : public cgon::object {
public:

	int red() { return _r; }
	void set_red(int r) { _r = r; }
	
	int green() { return _g; }
	void set_green(int g) { _g = g; }

	int blue() { return _b; }
	void set_blue(int b) { _b = b; }

	std::string name() { return get_name(); }

	using type_name = decltype("colour"_cgon_s);
	using child_types = cgon::type_list<>;
	using properties = cgon::type_list<
		cgon::property<decltype("r"_cgon_s), int, colour, &colour::red, &colour::set_red>,
		cgon::property<decltype("g"_cgon_s), int, colour, &colour::green, &colour::set_green>,
		cgon::property<decltype("b"_cgon_s), int, colour, &colour::blue, &colour::set_blue>
	>;

private:
	int _r, _g, _b;
};

class colour_list : public cgon::object {
public:
	colour_list() {
	}

	using type_name = decltype("colour"_cgon_s);
	using child_types = cgon::type_list<colour>;
	using properties = cgon::type_list<>;
};

int main() {
	cgon::document_schema<colour_list> colours_schema;
	
	std::unique_ptr<colour_list> colours =
		colours_schema.read_file("colours.cgon");

	for(colour* c : colours->children_of_type<colour>()) {
		std::cout << c->name() << ": " << c->red() << " " << c->green() << " " << c->blue() << std::endl;
	}
}
