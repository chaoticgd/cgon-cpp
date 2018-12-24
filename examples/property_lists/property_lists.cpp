#include <iostream>

#include <cgon/document.h>

struct config_difficulty {

	config_difficulty() {}

	config_difficulty(cgon::token_iterator& current) {
		std::string_view value_str = (current++)->value();
		if(value_str == "EASY") {
			value = EASY;
		} else if(value_str == "NORMAL") {
			value = NORMAL;
		} else if(value_str == "HARD") {
			value = HARD;
		} else {
			throw cgon::parse_error("Expected difficulty (EASY, MEDIUM or HARD)", current - 1);
		}
	}

	std::string to_string() {
		if(value == EASY) return "EASY";
		if(value == NORMAL) return "NORMAL";
		if(value == HARD) return "HARD";
		return "";
	}

	int value;
	enum { EASY = 0, NORMAL = 1, HARD = 2 };
};

struct config : public cgon::object {
	config_difficulty difficulty;
	std::tuple<int, int> window_size;
	bool fullscreen;

	using type_name = decltype("config"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::property_list<decltype("gameplay"_cgon_s), std::tuple<
			cgon::pointer_property<decltype("difficulty"_cgon_s), config_difficulty,
			                       config, &config::difficulty>
		>>,
		cgon::property_list<decltype("video"_cgon_s), std::tuple<
			cgon::pointer_property<decltype("window_size"_cgon_s), std::tuple<int, int>,
			                       config, &config::window_size>,
			cgon::pointer_property<decltype("fullscreen"_cgon_s), bool,
			                      config, &config::fullscreen>
		>>
	>;
};

int main() {
	std::unique_ptr<config> root =
		cgon::read_file<config>("property_lists.cgon");

	std::cout << "Config:\n"
	          << "\tDifficulty: "  << root->difficulty.to_string() << "\n"
	          << "\tWindow Size: " << std::get<0>(root->window_size)
	          << ", "              << std::get<1>(root->window_size) << "\n"
	          << "\tFullscreen: "  << (root->fullscreen ? "Yes" : "No") << "\n";
}
