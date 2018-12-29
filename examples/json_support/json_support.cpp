#include <iostream>
#include <cgon/document.h>

struct date {
	int year;
	int month;
	int day;
	
	using properties = std::tuple<
		cgon::pointer_property<decltype("year"_cgon_s), int,
		                       date, &date::year>,
		cgon::pointer_property<decltype("month"_cgon_s), int,
		                       date, &date::month>,
		cgon::pointer_property<decltype("day"_cgon_s), int,
		                       date, &date::day>
	>;

	std::string to_string() {
		// TODO: Add support for Y2K.
		return "19" + std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
	}
};

struct root_object { // get_name / set_name not required for JSON objects.
	std::string name;
	std::unique_ptr<date> date_of_birth;
	std::vector<std::string> hobbies;

	// type_name not required for JSON object.
	// child_types not required for JSON object.
	using properties = std::tuple<
		cgon::pointer_property<decltype("name"_cgon_s), std::string,
		                       root_object, &root_object::name>,
		cgon::pointer_property<decltype("date_of_birth"_cgon_s), std::unique_ptr<date>,
		                       root_object, &root_object::date_of_birth>,
		cgon::pointer_property<decltype("hobbies"_cgon_s), std::vector<std::string>,
		                       root_object, &root_object::hobbies>
	>;
};

int main() {
	std::unique_ptr<root_object> root =
		cgon::read_file<root_object, cgon::json_parser>("json_support.json");

	std::cout << root->name << "\n" << root->date_of_birth->to_string() << "\n";

	for(std::string hobby : root->hobbies) {
		std::cout << hobby << "\n";
	}
}
