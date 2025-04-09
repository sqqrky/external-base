#ifndef DRAWING_HPP
#define DRAWING_HPP

#include <dependencies/frameworks/imgui/imgui.h>

using vec2 = struct {
	float x, y;
};

using rect_t = struct {
	float x, y, height, width;
};

using rectangle_t = struct {

	rect_t object;
	uemath::flinearcolor color;
	vec2 radius;
	bool rounded;

};

using bounding_t = struct {
	float left, right, top, bottom;
	uemath::flinearcolor color;
	bool rounded;
};

using line_t = struct {
	vec2 point1, point2;
	uemath::flinearcolor color;
	float stroke;
};

using icon_t = struct {
	vec2 point;
	uemath::flinearcolor color;
	float fontSize;
	int icon;
};

using circle_t = struct {
	vec2 point;
	float radius;
	uemath::flinearcolor color;
	float stroke;
	bool filled;
};

using text_t = struct {

	std::uint32_t text_length;
	std::string text;
	vec2 point;
	uemath::flinearcolor color;
	bool center;
	bool menu;
	int weight;
};

namespace drawing {

	class c_drawing {

	public:

		explicit c_drawing() {};

		[[nodiscard]] uemath::flinearcolor get_color(std::int8_t r, std::int8_t g, std::int8_t b, std::int8_t a = 255);

		void text(ImDrawList* drawlist, int x, int y, uemath::flinearcolor color, const char* text, bool outlined);
		void rect(ImDrawList* draw_list, float x, float y, float width, float height, uemath::flinearcolor color, int thickness);
		void rect_cornered(ImDrawList* draw_list, int x, int y, int width, int height, uemath::flinearcolor color, int thickness);
		void line(float x1, float y1, float x2, float y2, uemath::flinearcolor color, float stroke = 1.0f);
		void circle(float x, float y, float radius, uemath::flinearcolor color, float stroke, bool filled = false);

		void crosshair();
		void fov();
		void skeleton(ImDrawList* draw_list, const uemath::fvector2d* bone_positions, const std::pair<int, int>* bone_pairs, int bone_pair_count, ImU32 color, float thickness);
	};
} inline auto drawing_data = std::make_unique<drawing::c_drawing>();

#endif