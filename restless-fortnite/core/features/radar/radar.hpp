#ifndef RADAR_HPP
#define RADAR_HPP

#include <memory>
#include <core/sdk/math/math.hpp>
#include <dependencies/frameworks/imgui/imgui.h>

namespace radar {
	class c_radar
	{
	public:
		static void draw_radar(ImDrawList* draw_list);
		static void range_to(float* x, float* y, float range);
		static void calculate_radar_point(uemath::fvector origin, int& screen_x, int& screen_y);
		static void add_radar(ImDrawList* draw_list, uemath::fvector world_loc, ImColor im_color, std::string distance);
	};
} inline const auto radar_data = std::make_unique<radar::c_radar>();

#endif