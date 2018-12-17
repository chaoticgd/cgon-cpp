# Writing Schemas

## Introduction

Schemas are C++ classes that represent object types in CGON files. They specify the type name (used to reference the class from CGON), the types of the children and the properties that a certain object can take.

For example, consider the following CGON object:

```cgon
colour_list {
	colour red         { r: 255 g: 0   b: 0   }
	colour yellow      { r: 255 g: 255 b: 0   }
	
	...

	colour transparent { r: 0   g: 0   b: 0   a: 0 }
}
```

To parse this file, two schema classes are required. One corresponding to the colour_list object, the other corresponding to the colour objects. Lets look at how the colour schema class is defined:

```cpp
class colour_list : public cgon::object {
public:
	using type_name = decltype("colour"_cgon_s);
	using child_types = std::tuple<colour>;
	using properties = std::tuple<>;
};
```

Note that inheriting from cgon::object is not a requirement. If the object type allows children, an append_child member function must be provided compatible with the one in include/cgon/tree.h. Additionally, get_name and set_name member functions must be defined regardless of if the object can have children.

The first using declaration specifies the name of the object type as referred to from CGON. The second specifies the types of children that objects of this type can have. Multiple child types can be specified by adding them as additional template parameters to the std::tuple. The third declaration will be explained later.

## Properties

Now lets inspect the definition of the colour schema:

```cpp
class colour : public cgon::object {
public:

	...

	using type_name = decltype("colour"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::property<decltype("r"_cgon_s), int,
		               colour, &colour::get_red, &colour::set_red>,
		cgon::property<decltype("g"_cgon_s), int,
		               colour, &colour::get_green, &colour::set_green>,
		cgon::property<decltype("b"_cgon_s), int,
		               colour, &colour::get_blue, &colour::set_blue>,
		cgon::property<decltype("a"_cgon_s), std::optional<int>,
		               colour, &colour::get_alpha, &colour::set_alpha>
	>;

	...

};
```

Each template parameter passed to the std::tuple in the third using declaration specifies a different property. The first template parameter that cgon::property takes specifies the name of the property, the second specifies the type. The third parameter must be equal to the type of the schema class for implementation-specific reasons. The last two parameters are pointers to member functions of the schema class: a getter and a setter respecitvely. The definitions of these getters/setters are omitted in the above section of code.

Properties can be of the following types: int (and variations thereof e.g. unsigned long), float, double, std::string, std::optional<T>, std::vector<T>, std::array<T, T_index>, std::tuple<T1, T2, ...> where T is any valid property type.
