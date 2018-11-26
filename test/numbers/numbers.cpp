#include "gtest/gtest.h"

#include <iostream>
#include <cgon/document.h>
#include <cgon/value_property.h>

class numbers : public cgon::object {
public:
	numbers() : a(this, "a"), b(this, "b"), c(this, "c"), d(this, "d") {}

	static std::string type_name() { return "numbers"; }

	cgon::value_property<int> a, b, c;
	cgon::value_property<double> d;
};

TEST(numbers, main) {
	cgon::document_schema<numbers> colours_schema;

	std::unique_ptr<numbers> root =
		colours_schema.read_file("numbers/numbers.cgon");

	EXPECT_EQ(root->a.get(), 1337);
	EXPECT_EQ(root->b.get(), 123456789);
	EXPECT_EQ(root->c.get(), -32);
	EXPECT_DOUBLE_EQ(root->d.get(), 42.42);
}