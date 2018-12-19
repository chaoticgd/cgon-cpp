# Custom Property Types

In addition to the types listed in [writing-schemas.md](writing-schemas.md), properties may be of any user-defined type that implements X::X(cgon::token_iterator&) where X is the user-defined type. For example:

```cpp
struct iso8601_date {

	iso8601_date() : year(0), month(0), day(0) {}

	iso8601_date(cgon::token_iterator& current) {
		std::string value = (current++)->value();
		year = std::stoi(value.substr(0, 4));
		month = std::stoi(value.substr(5, 7));
		day = std::stoi(value.substr(8, 10));
	}

	...

	int year;
	int month;
	int day;
};
```

The argument passed to the constructor is an iterator pointing into an std::vector of [tokens](https://en.wikipedia.org/wiki/Lexical_analysis#Token). Each token is its own unit of syntax. For example, `"items { item { message: 'hello world' }}"` would be broken down into { `"items"`, `"{"`, `"item"`, `"{"`, `"message"`, `":"`, `"'hello world'"`, `"}"`, `"}"` }.

When the constructor is called `current` will point to the first token after the property name delimeter (the colon). When the constructor returns, `current` must point to the next token after the property definition. For example, when `iso8601_date::iso8601_date` in the above example is called, the highlighted token will be pointed to by `current`:


`{	`**_`property`_**`: 2018-02-03 }`

When the constructor returns, the following highlighted token should be pointed to by `current`:

`{	property: 2018-02-03 `**_`}`_**

If an unexpected end of file is encountered, `cgon::token_iterator` will throw an exception. You don't have to deal with this yourself.

All of this would let dates be written in a CGON file like so:

```cpp
date: 2017-04-02
```

In a real-world program, more error checking would be required. If you encounter an invalid token (e.g. you're expecting a number but a string is given) you should throw a `cgon::parse_error`. The first argument given to the constructor of `cgon::parse_error` is an error message. The second is the problematic token. For example, from examples/custom_properties/custom_properties.cpp:

```cpp
if(value < 0) {
	throw cgon::parse_error("The price cannot be negative", current - 2);
}
```

would result in the following error if a negative price is provided:

```
  what():  The price cannot be negative:
	id : VF-3528 
price : -33 GBP 
date : 2018-11-20 
```
