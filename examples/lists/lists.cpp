#include <iostream>
#include <cgon/document_schema.h>

class lists : public cgon::object {
public:

	std::vector<int> get_primes() { return primes; }
	void set_primes(std::vector<int> p) { primes = p; };
	
	std::vector<std::vector<int>> get_multi_dimensional() { return multi_dimensional; }
	void set_multi_dimensional(std::vector<std::vector<int>> m) { multi_dimensional = m; }

	std::vector<std::string> get_strings() { return strings; }
	void set_strings(std::vector<std::string> s) { strings = s; }

	using type_name = decltype("lists"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::property<decltype("primes"_cgon_s), std::vector<int>,
		               lists, &lists::get_primes, &lists::set_primes>,
		cgon::property<decltype("multi_dimensional"_cgon_s), std::vector<std::vector<int>>,
		               lists, &lists::get_multi_dimensional, &lists::set_multi_dimensional>,
		cgon::property<decltype("strings"_cgon_s), std::vector<std::string>,
		               lists, &lists::get_strings, &lists::set_strings>
	>;

private:
	std::vector<int> primes;
	std::vector<std::vector<int>> multi_dimensional;
	std::vector<std::string> strings;
};

int main() {
	cgon::document_schema<lists> lists_schema;

	std::unique_ptr<lists> lists =
		lists_schema.read_file("lists.cgon");

	for(int prime : lists->get_primes()) {
		std::cout << prime << " ";
	}
	std::cout << std::endl;

	for(auto sub_list : lists->get_multi_dimensional()) {
		for(auto number : sub_list) {
			std::cout << number << " ";
		}
		std::cout << std::endl;
	}

	for(auto string : lists->get_strings()) {
		std::cout << string << std::endl;
	}
}
