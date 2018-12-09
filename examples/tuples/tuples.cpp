#include <iostream>
#include <cgon/document_schema.h>

typedef std::tuple<std::string, std::string, int> user;

class tuples_example : public cgon::object {
public:

	std::vector<user> get_users() { return users; }
	void set_users(std::vector<user> u) { users = u; };
	
	using type_name = decltype("tuples_example"_cgon_s);
	using child_types = std::tuple<>;
	using properties = cgon::type_list<
		cgon::property<decltype("users"_cgon_s), std::vector<user>,
		               tuples_example, &tuples_example::get_users, &tuples_example::set_users>
	>;

private:
	std::vector<user> users;
};

int main() {
	cgon::document_schema<tuples_example> lists_schema;

	std::unique_ptr<tuples_example> lists =
		lists_schema.read_file("tuples.cgon");

	for(user current : lists->get_users()) {
		std::cout << std::get<0>(current) << " has password " << std::get<1>(current) << " and DoB " << std::get<2>(current) << "\n";
	}
}
