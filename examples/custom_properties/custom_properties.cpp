#include <iostream>
#include <cgon/document_schema.h>
#include <cgon/token.h>

struct transaction_id {

	transaction_id() : start({' ', ' '}), end(0) {}

	transaction_id(cgon::token_iterator& current) {
		std::string value = (current++)->copy_value();
		
		if(value.size() != 7 || value[2] != '-') {
			throw cgon::parse_error("Invalid ID format", current - 1);
		}

		start[0] = value[0];
		start[1] = value[1];
		end = std::stod(value.substr(3));
	}

	std::string to_string() {
		return std::string() + start[0] + start[1] + "-" + std::to_string(end);
	}

	std::array<char, 2> start;
	int end;
};

enum class transaction_currency {
	USD, GDB, BTC
};

struct transaction_price {

	transaction_price() : value(0), currency(transaction_currency::BTC) {}

	transaction_price(cgon::token_iterator& current) {
		std::string value_str = (current++)->copy_value();
		std::string_view currency_str = (current++)->value();

		value = std::stod(value_str);
		if(value < 0) {
			throw cgon::parse_error("The price cannot be negative", current - 2);
		}
		for(auto pair : currency_strings) {
			if(currency_str == pair.second) {
				currency = pair.first;
			}
		}
	}

	transaction_price& operator=(const transaction_price& rhs) {
		value = rhs.value;
		currency = rhs.currency;
		return *this;
	}

	std::string to_string() {
		return std::to_string(value) + currency_strings.at(currency);
	}

	float value;
	transaction_currency currency;

private:
	const std::map<transaction_currency, std::string> currency_strings {
		{ transaction_currency::USD, "USD" },
		{ transaction_currency::GDB, "GBP" },
		{ transaction_currency::BTC, "BTC" }
	};
};

struct iso8601_date {

	iso8601_date() : year(0), month(0), day(0) {}

	iso8601_date(cgon::token_iterator& current) {
		std::string value = (current++)->copy_value();
		year = std::stoi(value.substr(0, 4));
		month = std::stoi(value.substr(5, 7));
		day = std::stoi(value.substr(8, 10));
	}

	std::string to_string() {
		return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
	}

	int year;
	int month;
	int day;
};

struct transaction : public cgon::object {
	
	transaction_id id;
	transaction_price price;
	iso8601_date date;

	using type_name = decltype("transaction"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::pointer_property<decltype("id"_cgon_s), transaction_id,
		                       transaction, &transaction::id>,
		cgon::pointer_property<decltype("price"_cgon_s), transaction_price,
		                       transaction, &transaction::price>,
		cgon::pointer_property<decltype("date"_cgon_s), iso8601_date,
		                       transaction, &transaction::date>
	>;
};

struct transactions : public cgon::object {
	using type_name = decltype("transactions"_cgon_s);
	using child_types = std::tuple<transaction>;
	using properties = std::tuple<>;
};

int main() {
	cgon::document_schema<transactions> colours_schema;
	
	std::unique_ptr<transactions> colours =
		colours_schema.read_file("custom_properties.cgon");

	for(transaction* t : colours->children_of_type<transaction>()) {
		std::cout << "Transaction " << t->id.to_string()
		          << " with price " << t->price.to_string()
		          << " on date " << t->date.to_string() << "\n";
	}
}
