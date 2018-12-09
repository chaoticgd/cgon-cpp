#include <iostream>
#include <cgon/document_schema.h>

class colour : public cgon::object {
public:

	int get_red() { return red; }
	void set_red(int r) { red = r; }
	
	int get_green() { return green; }
	void set_green(int g) { green = g; }

	int get_blue() { return blue; }
	void set_blue(int b) { blue = b; }

	std::optional<int> get_alpha() { return alpha; }
	void set_alpha(std::optional<int> a) { alpha = a; }

	std::string name() { return get_name(); }

	using type_name = decltype("colour"_cgon_s);
	using child_types = std::tuple<>;
	using properties = cgon::type_list<
		cgon::property<decltype("r"_cgon_s), int, colour, &colour::get_red, &colour::set_red>,
		cgon::property<decltype("g"_cgon_s), int, colour, &colour::get_green, &colour::set_green>,
		cgon::property<decltype("b"_cgon_s), int, colour, &colour::get_blue, &colour::set_blue>,
		cgon::property<decltype("a"_cgon_s), std::optional<int>, colour, &colour::get_alpha, &colour::set_alpha>
	>;

private:
	int red, green, blue;
	std::optional<int> alpha;
};

class colour_list : public cgon::object {
public:
	colour_list() {
	}

	using type_name = decltype("colour"_cgon_s);
	using child_types = std::tuple<colour>;
	using properties = cgon::type_list<>;
};

int main() {
	cgon::document_schema<colour_list> colours_schema;
	
	std::unique_ptr<colour_list> colours =
		colours_schema.read_file("colours.cgon");

	for(colour* c : colours->children_of_type<colour>()) {
		std::cout << c->name() << ": " << c->get_red() << " " << c->get_green() << " " << c->get_blue();
		if(c->get_alpha().has_value()) {
			std::cout << " " << c->get_alpha().value();
		}
		std::cout << "\n";
	}
}
