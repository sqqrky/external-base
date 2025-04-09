#ifndef VISUALS_CPP
#define VISUALS_CPP

#include <chrono>
#include <core/includes.hpp>

static std::chrono::steady_clock::time_point last_click_time = std::chrono::steady_clock::now();
static std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();
static int frames = 0;
static float fps = 0.0f;

void visuals::c_visuals::loop() {
	auto draw_list = ImGui::GetBackgroundDrawList();

	entity::c_entity closest_entity = { };
	auto target_distance = FLT_MAX;

	if (g_vars->misc.watermark.fps) {
		++frames;
		auto now = std::chrono::steady_clock::now();
		float duration = std::chrono::duration<float>(now - last_time).count();
		if (duration >= 0.5f) {
			fps = frames / duration;
			frames = 0.0f;
			last_time = now;
		}

		std::string fps_format = hash_str("fps: ") + std::to_string(static_cast<int>(fps));
		drawing_data->text(draw_list, 10.0f, 10.0f, uemath::flinearcolor(255, 255, 255, 255), fps_format.c_str(), g_vars->visuals.other.outlined_text);
	}

	uengine::update_matrix();

	auto rotation_matrix = uengine::create_rotation_matrix(uengine::camera_data->rotation);
	auto aspect_ratio = static_cast<float>(uemath::m_width) / uemath::m_height;
	float adjusted_fov = aspect_ratio / (16.0f / 9.0f) * tanf(uengine::camera_data->fov * static_cast<float>(std::numbers::pi) / 360.0f);

	for (auto& entity : m_entity_cache) {

		std::uintptr_t bone_space_trans = kernel->read<std::uintptr_t>(entity.mesh + offsets->bone_space_transforms);
		if (!bone_space_trans)
			bone_space_trans = kernel->read<std::uintptr_t>(entity.mesh + offsets->bone_space_transforms + 0x10);
		auto comp_to_world = kernel->read<uengine::ftransform>(entity.mesh + offsets->component_to_world);

		auto head_bone = uengine::get_bone_data(110, bone_space_trans, comp_to_world);
		auto root_bone = uengine::get_bone_data(0, bone_space_trans, comp_to_world);

		auto in_lobby = kernel->read<float>(world_data->game_state + offsets->server_world_time);
		auto distance = uengine::camera_data->location.distance(root_bone) / 100;
		if (!in_lobby && distance >= g_vars->visuals.max_distance)
			return;

		auto m_distance = in_lobby ? 10.0f : distance;
		if (!m_distance)
			continue;

		auto head_screen = uengine::world_to_screen(head_bone, rotation_matrix, adjusted_fov);
		auto head_box = uengine::world_to_screen({ head_bone.x, head_bone.y, head_bone.z + 15 }, rotation_matrix, adjusted_fov);
		auto root_screen = uengine::world_to_screen(root_bone, rotation_matrix, adjusted_fov);
		auto box_height = std::abs(head_box.y - root_screen.y);
		auto box_width = box_height * 0.45f;

		if (g_vars->visuals.box)
			drawing_data->rect(draw_list, head_box.x - (box_width * 0.5f), head_box.y, box_width, box_height, uemath::flinearcolor(255, 255, 255, 255), g_vars->visuals.box_thickness);

		if (g_vars->visuals.bounding_box) {
			const std::vector<int> bone_indices = { 110, 71, 78, 72, 79, 83, 76, 9, 38, 33, 62 };
			std::vector<uemath::fvector2d> bone_screen_coords;
			bone_screen_coords.reserve(bone_indices.size());

			for (int index : bone_indices) {
				auto bone_pos = uengine::get_bone_data(index, bone_space_trans, comp_to_world);
				bone_screen_coords.push_back(uengine::world_to_screen(bone_pos, rotation_matrix, adjusted_fov));
			}

			auto [min_x, max_x] = std::minmax_element(bone_screen_coords.begin(), bone_screen_coords.end(), [](const auto& a, const auto& b) { return a.x < b.x; });
			auto [min_y, max_y] = std::minmax_element(bone_screen_coords.begin(), bone_screen_coords.end(), [](const auto& a, const auto& b) { return a.y < b.y; });

			const auto box_height = max_y->y - min_y->y;
			const auto box_width = max_x->x - min_x->x;

			const auto width_offset = box_width * 0.175f;
			const auto height_offset_top = box_height * 0.125f;
			const auto height_offset_bottom = box_height * 0.05f;

			ImVec2 top_left(min_x->x - width_offset, min_y->y - height_offset_top);
			ImVec2 bottom_right(max_x->x + width_offset, max_y->y + height_offset_bottom);

			drawing_data->rect(draw_list, top_left.x, top_left.y, bottom_right.x - top_left.x, bottom_right.y - top_left.y,
				uemath::flinearcolor(255, 255, 255, 255), g_vars->visuals.box_thickness);
		}

		if (g_vars->visuals.distance) {
			std::string distance_format = hash_str("[") + std::to_string(static_cast<int>(m_distance)) + hash_str("m]");
			drawing_data->text(draw_list,
				root_screen.x, root_screen.y,
				uemath::flinearcolor(255, 255, 255, 255),
				distance_format.c_str(), 0
			);
		}

		// hardcoded bounding boxes so tuff (js use getbounds)

		if (g_vars->visuals.weapon) {
			auto current_weapon = kernel->read<std::uintptr_t>(entity.actor + offsets->current_weapon);
			std::string weapon = uengine::get_weapon_name(current_weapon);
			if (g_vars->visuals.ammo) {
				auto ammo_count = kernel->read<std::int32_t>(current_weapon + offsets->ammo_count);
				weapon += hash_str("[") + std::to_string(ammo_count) + hash_str("]");
			}
			auto text_size = ImGui::CalcTextSize(weapon.c_str());
			drawing_data->text(draw_list, head_box.x - text_size.x * 0.5f, head_box.y - 15, uemath::flinearcolor(255, 255, 255, 255), weapon.c_str(), g_vars->visuals.other.outlined_text);
		}

		// use weaponcoreanimation over this

		if (g_vars->visuals.username) {
			int offset = g_vars->visuals.weapon ? 30 : 15;
			auto text_size = ImGui::CalcTextSize(entity.user_name.c_str());
			drawing_data->text(draw_list, head_box.x - text_size.x * 0.5f, head_box.y - offset, uemath::flinearcolor(255, 255, 255, 255), entity.user_name.c_str(), g_vars->visuals.other.outlined_text);
		}

		if (g_vars->visuals.skeleton) {
			const std::pair<int, int> bone_pairs[] = {
				{1, 2}, {3, 2}, {2, 4}, {5, 3}, {6, 4}, {5, 7}, {6, 8}, {10, 1}, {9, 1},
				{12, 10}, {11, 9}, {13, 12}, {14, 11}, {2, 15}, {71, 72}, {72, 83}, {74, 75}, {75, 76}
			};

			std::vector<uemath::fvector2d> bone_positions;
			for (int i = 0; i < std::size(bone_pairs); ++i) {
				auto bone_pos = uengine::get_bone_data(bone_pairs[i].first, bone_space_trans, comp_to_world);
				auto bone_screen = uengine::world_to_screen(bone_pos, rotation_matrix, adjusted_fov);
				bone_positions.push_back(bone_screen);
			}

			drawing_data->skeleton(
				draw_list,
				bone_positions.data(),
				bone_pairs,
				std::size(bone_pairs),
				IM_COL32(255, 255, 255, 255),
				0.8f
			);

			// cut 'em out my life, just like a photo, i just crop 'em
			// also this code sucks
		}

		if (g_vars->visuals.radar.enabled) {
		}

		float dx = head_screen.x - uemath::m_width_center;
		float dy = head_screen.y - uemath::m_height_center;
		float dist_to_center = std::sqrt(dx * dx + dy * dy);

		//if (g_vars->aimbot.prediction && uengine::gravity > 0.0f) {
		//}

		//if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) && g_vars->aimbot.enabled && !predicted_pos.is_zero() && aimbot->get_cross_distance(predicted_pos.x, predicted_pos.y) <= g_vars->aimbot.fov)
			//aimbot->aim_lock(predicted_pos);

		// i removed all of the code surrounding this shit because i'm not fixing it
		// have a field day doing that yourself
	}
}

void visuals::c_visuals::render() {
	// put all the render esp inside of the loop func  
	// nothing else should be defined here, all we're doing is calling the render func

	this->loop();
}

#endif