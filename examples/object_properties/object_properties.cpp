#include <iostream>

#include <cgon/document.h>

struct config_difficulty {

	config_difficulty() {}

	template <typename T_parser>
	void parse(cgon::token_iterator& current) {
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

struct config_gameplay : public cgon::object {
	config_difficulty difficulty;

	using type_name = decltype("config_gameplay"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::pointer_property<decltype("difficulty"_cgon_s), config_difficulty,
		                       config_gameplay, &config_gameplay::difficulty>
	>;
};

struct config_video : public cgon::object {
	std::tuple<int, int> window_size;
	bool fullscreen;

	using type_name = decltype("config_video"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::pointer_property<decltype("window_size"_cgon_s), std::tuple<int, int>,
		                       config_video, &config_video::window_size>,
		cgon::pointer_property<decltype("fullscreen"_cgon_s), bool,
		                       config_video, &config_video::fullscreen>
	>;
};

struct config : public cgon::object {
	std::unique_ptr<config_gameplay> gameplay;
	std::unique_ptr<config_video> video;

	using type_name = decltype("config"_cgon_s);
	using child_types = std::tuple<>;
	using properties = std::tuple<
		cgon::pointer_property<decltype("gameplay"_cgon_s), std::unique_ptr<config_gameplay>,
		                       config, &config::gameplay>,
		cgon::pointer_property<decltype("video"_cgon_s), std::unique_ptr<config_video>,
		                       config, &config::video>
	>;
};

int main() {
	std::unique_ptr<config> root =
		cgon::read_file<config>("object_properties.cgon");

	std::cout << "Config:\n"
	           << "\tDifficulty: "  << root->gameplay->difficulty.to_string() << "\n"
	           << "\tWindow Size: " << std::get<0>(root->video->window_size)
	           << ", "              << std::get<1>(root->video->window_size) << "\n"
	           << "\tFullscreen: "  << (root->video->fullscreen ? "Yes" : "No") << "\n";
}
