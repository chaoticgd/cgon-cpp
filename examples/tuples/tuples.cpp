#include <iostream>
#include <cgon/document.h>

typedef std::tuple<std::string, std::string, int> user;

class tuples_example : public cgon::object {
public:

	std::vector<user> get_users() { return users; }
	void set_users(std::vector<user> u) { users = u; };
	
	using type_name = decltype("tuples_example"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::property<decltype("users"_cgon_s), std::vector<user>,
		               tuples_example, &tuples_example::get_users, &tuples_example::set_users>
	>;

private:
	std::vector<user> users;
};

int main() {
	std::unique_ptr<tuples_example> lists =
		cgon::read_file<tuples_example>("tuples.cgon");

	for(user current : lists->get_users()) {
		std::cout << std::get<0>(current) << " has password " << std::get<1>(current) << " and DoB " << std::get<2>(current) << "\n";
	}
}
