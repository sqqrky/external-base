#ifndef DRAWING_CPP
#define DRAWING_CPP

#include <core/includes.hpp>

auto drawing::c_drawing::get_color(std::int8_t r, std::int8_t g, std::int8_t b, std::int8_t a) -> uemath::flinearcolor {

	return uemath::flinearcolor{
		static_cast<std::int8_t>(a),
		static_cast<std::int8_t>(r),
		static_cast<std::int8_t>(g),
		static_cast<std::int8_t>(b)
	};

}

void drawing::c_drawing::circle(float x, float y, float radius, uemath::flinearcolor color, float stroke, bool filled) {
	const auto draw_list = ImGui::GetBackgroundDrawList();

	ImU32 im_color = IM_COL32(color.r, color.g, color.b, color.a);

	if (filled) {
		draw_list->AddCircleFilled(
			ImVec2(x, y),
			radius,
			im_color,
			12
		);
	}
	else {
		draw_list->AddCircle(
			ImVec2(x, y),
			radius,
			im_color,
			12,
			stroke
		);
	}
}

void drawing::c_drawing::text(
	ImDrawList* drawlist,
	int x,
	int y,
	uemath::flinearcolor color,
	const char* text,
	bool outlined) {

	if (outlined)
	{
		ImVec2 offsets[] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

		for (const auto& offset : offsets)
		{
			drawlist->AddText(ImVec2(x + offset.x, y + offset.y), ImColor(0, 0, 0, 255), text);
		}
	}
	ImU32 im_color = IM_COL32(color.r, color.g, color.b, color.a);
	drawlist->AddText(ImVec2(x, y), im_color, text);
}

void drawing::c_drawing::line(float x1, float y1, float x2, float y2, uemath::flinearcolor color, float stroke) {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImU32 im_color = IM_COL32(color.r, color.g, color.b, color.a);
	draw_list->AddLine(
		ImVec2(x1, y1),
		ImVec2(x2, y2),
		im_color,
		stroke
	);
}

auto drawing::c_drawing::crosshair() -> void
{
	this->line(
		uemath::m_width - 5,
		uemath::m_height,
		uemath::m_width + 5,
		uemath::m_height,
		this->get_color(255, 255, 255, 255)
	);

	this->line(
		uemath::m_width,
		uemath::m_height - 5,
		uemath::m_width,
		uemath::m_height + 5,
		this->get_color(255, 255, 255, 255)
	);
}

auto drawing::c_drawing::fov() -> void
{
	this->circle(
		uemath::m_width, uemath::m_height,
		g_vars->aimbot.fov,
		this->get_color(255, 255, 255, 255),
		1.f
	);
}

void drawing::c_drawing::rect(
	ImDrawList* draw_list,
	float x,
	float y,
	float width,
	float height,
	uemath::flinearcolor color,
	int thickness
) {

	if (g_vars->visuals.other.filled) {
		ImU32 im_color = IM_COL32(color.r, color.g, color.b, color.a);
		draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + width, y + height), ImColor(0, 0, 0, 85), 0, 0);
	}

	if (g_vars->visuals.other.outlined) {
		ImU32 im_color = IM_COL32(color.r, color.g, color.b, color.a);
		draw_list->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + width + 1, y + height + 1), ImColor(0, 0, 0, 255), 0, 0, thickness);
	}

	ImU32 im_color = IM_COL32(color.r, color.g, color.b, color.a);
	draw_list->AddRect(ImVec2(x, y), ImVec2(x + width, y + height), im_color, 0, 0, thickness);
}

void drawing::c_drawing::rect_cornered(ImDrawList* draw_list, int x, int y, int width, int height, uemath::flinearcolor color, int thickness)
{
	float line_width = static_cast<float>(width) / 3.0f;
	float line_height = static_cast<float>(height) / 20.0f;
	float scaled_thickness = static_cast<float>(thickness);

	if (g_vars->visuals.other.filled) {
		draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + width, y + height), ImColor(0, 0, 0, 85));
	}

	if (g_vars->visuals.other.outlined) {
		draw_list->AddLine(ImVec2(x - scaled_thickness, y - scaled_thickness), ImVec2(x + line_width + scaled_thickness, y - scaled_thickness), ImColor(0, 0, 0, 255), scaled_thickness);
		draw_list->AddLine(ImVec2(x - scaled_thickness, y - scaled_thickness), ImVec2(x - scaled_thickness, y + line_height + scaled_thickness), ImColor(0, 0, 0, 255), scaled_thickness);
		draw_list->AddLine(ImVec2(x + width - line_width - scaled_thickness, y - scaled_thickness), ImVec2(x + width + scaled_thickness, y - scaled_thickness), ImColor(0, 0, 0, 255), scaled_thickness);
		draw_list->AddLine(ImVec2(x + width + scaled_thickness, y - scaled_thickness), ImVec2(x + width + scaled_thickness, y + line_height + scaled_thickness), ImColor(0, 0, 0, 255), scaled_thickness);

		draw_list->AddLine(ImVec2(x - scaled_thickness, y + height - line_height - scaled_thickness), ImVec2(x - scaled_thickness, y + height + scaled_thickness), ImColor(0, 0, 0, 255), scaled_thickness);
		draw_list->AddLine(ImVec2(x - scaled_thickness, y + height + scaled_thickness), ImVec2(x + line_width + scaled_thickness, y + height + scaled_thickness), ImColor(0, 0, 0, 255), scaled_thickness);
		draw_list->AddLine(ImVec2(x + width - line_width - scaled_thickness, y + height - scaled_thickness), ImVec2(x + width + scaled_thickness, y + height - scaled_thickness), ImColor(0, 0, 0, 255), scaled_thickness);
		draw_list->AddLine(ImVec2(x + width + scaled_thickness, y + height - line_height - scaled_thickness), ImVec2(x + width + scaled_thickness, y + height + scaled_thickness), ImColor(0, 0, 0, 255), scaled_thickness);
	}

	ImU32 im_color = IM_COL32(color.r, color.g, color.b, color.a);

	draw_list->AddLine(ImVec2(x - scaled_thickness, y - scaled_thickness), ImVec2(x + line_width + scaled_thickness, y - scaled_thickness), im_color, scaled_thickness);
	draw_list->AddLine(ImVec2(x - scaled_thickness, y - scaled_thickness), ImVec2(x - scaled_thickness, y + line_height + scaled_thickness), im_color, scaled_thickness);
	draw_list->AddLine(ImVec2(x + width - line_width - scaled_thickness, y - scaled_thickness), ImVec2(x + width + scaled_thickness, y - scaled_thickness), im_color, scaled_thickness);
	draw_list->AddLine(ImVec2(x + width + scaled_thickness, y - scaled_thickness), ImVec2(x + width + scaled_thickness, y + line_height + scaled_thickness), im_color, scaled_thickness);

	draw_list->AddLine(ImVec2(x - scaled_thickness, y + height - line_height - scaled_thickness), ImVec2(x - scaled_thickness, y + height + scaled_thickness), im_color, scaled_thickness);
	draw_list->AddLine(ImVec2(x - scaled_thickness, y + height + scaled_thickness), ImVec2(x + line_width + scaled_thickness, y + height + scaled_thickness), im_color, scaled_thickness);
	draw_list->AddLine(ImVec2(x + width - line_width - scaled_thickness, y + height - scaled_thickness), ImVec2(x + width + scaled_thickness, y + height - scaled_thickness), im_color, scaled_thickness);
	draw_list->AddLine(ImVec2(x + width + scaled_thickness, y + height - line_height - scaled_thickness), ImVec2(x + width + scaled_thickness, y + height + scaled_thickness), im_color, scaled_thickness);
}

void drawing::c_drawing::skeleton(
	ImDrawList* draw_list,
	const uemath::fvector2d* bone_positions,
	const std::pair<int,
	int>* bone_pairs,
	int bone_pair_count,
	ImU32 color,
	float thickness
 ){
	 // nayte ashlirdi is so damn tuff boi
	draw_list->PushClipRectFullScreen();

	for (int i = 0; i < bone_pair_count; ++i) {
		const auto& pair = bone_pairs[i];
		ImVec2 start(
			bone_positions[pair.first].x,
			bone_positions[pair.first].y
		);

		ImVec2 end(
			bone_positions[pair.second].x,
			bone_positions[pair.second].y
		);

		if (start.x == 0.0f && start.y == 0.0f) continue;

		if (end.x == 0.0f && end.y == 0.0f) continue;

		draw_list->AddLine(
			start,
			end,
			color,
			thickness
		);
	}
	draw_list->PopClipRect();
}

// where tf did this come from again v? i didn't make ts :sob:

#endif