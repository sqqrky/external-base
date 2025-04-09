#pragma once 

#include <core/includes.hpp>

auto uengine::update_matrix() -> void {

	auto projection = kernel->read< uengine::fmatrix >(world_data->view_state.get(1) + 0x940);

	camera_data->rotation.pitch = std::asin(projection.z_plane.w) * rad_to_deg;

	camera_data->rotation.yaw = std::atan2(projection.y_plane.w, projection.x_plane.w) * rad_to_deg;

	camera_data->rotation.roll = 0.0f;

	camera_data->location.x = projection.m[3][0];
	camera_data->location.y = projection.m[3][1];
	camera_data->location.z = projection.m[3][2];

	auto fov_radians = 2.0f * std::atanf(1.0f / static_cast<float>(kernel->read< double >(world_data->view_state.get(1) + 0x740)));
	camera_data->fov = fov_radians * rad_to_deg;
}

uemath::fvector uengine::get_bone_data(int bone_index, uintptr_t bone_array_ptr, uengine::ftransform component_to_world)
{
	uengine::ftransform bone = kernel->read<uengine::ftransform>(bone_array_ptr + (bone_index * sizeof(uengine::ftransform)));

	D3DMATRIX matrix = uengine::matrix_multiplication(bone.to_matrix_with_scale(), component_to_world.to_matrix_with_scale());

	return uemath::fvector(matrix._41, matrix._42, matrix._43);
}

std::vector<uemath::fvector> uengine::get_all_bone_data(const int* bone_indices, size_t num_bones, uintptr_t bone_array_ptr, uengine::ftransform component_to_world) {
	std::vector<uemath::fvector> bone_positions;
	bone_positions.reserve(num_bones);

	for (size_t i = 0; i < num_bones; ++i) {
		uemath::fvector bone_position = get_bone_data(bone_indices[i], bone_array_ptr, component_to_world);
		bone_positions.push_back(bone_position);
	}

	return bone_positions;
}


uemath::fvector2d uengine::world_to_screen(uemath::fvector location, D3DMATRIX rotation_matrix, float adjusted_fov) {
	auto& axis_x = rotation_matrix.m[0];
	auto& axis_y = rotation_matrix.m[1];
	auto& axis_z = rotation_matrix.m[2];

	uemath::fvector delta = location - camera_data->location;

	auto transformed_x = delta.dot(uemath::fvector(axis_y[0], axis_y[1], axis_y[2]));
	auto transformed_y = delta.dot(uemath::fvector(axis_z[0], axis_z[1], axis_z[2]));
	auto transformed_z = max(delta.dot(uemath::fvector(axis_x[0], axis_x[1], axis_x[2])), 1.0f);

	auto screen_x = uemath::m_width_center + transformed_x * (uemath::m_width_center / adjusted_fov) / transformed_z;
	auto screen_y = uemath::m_height_center - transformed_y * (uemath::m_width_center / adjusted_fov) / transformed_z;

	return uemath::fvector2d(screen_x, screen_y);
}

std::string uengine::decrypt_name(std::uintptr_t state) {
	__int64 f_string = 0;
	int length = 0;
	std::uintptr_t f_text = 0;

	// remove this server_time read since it's already read in the render func
	// you can just add a sep param or smt to identify

	float server_time = kernel->read<float>(world_data->game_state + 0x2e0);
	if (server_time == 0.0f) {
		length = kernel->read <int>(state + 0x348); 
		f_text = kernel->read <__int64>(state + 0x340);
	}
	else {
		f_string = kernel->read <__int64>(state + 0xb08);
		length = kernel->read <int>(f_string + 16);
		f_text = kernel->read <__int64>(f_string + 8);
	}

	if (length > 0 && length < 50) {
		__int64 v6 = length;
		if (!v6) return std::string(hash_str(""));

		wchar_t* buffer = new wchar_t[length];
		kernel->read_physical((void*)f_text, buffer, length * sizeof(wchar_t));

		char v21;
		int v22;
		int i;
		int v25;
		WORD* v23;

		v21 = v6 - 1;
		if (!(DWORD)v6)
			v21 = 0;
		v22 = 0;
		v23 = (WORD*)buffer;
		for (i = (v21) & 3; ; *v23++ += i & 7)
		{
			v25 = v6 - 1;
			if (!(DWORD)v6)
				v25 = 0;
			if (v22 >= v25)
				break;
			i += 3;
			++v22;
		}

		std::wstring username{ buffer };

		delete[] buffer;

		return std::string(username.begin(), username.end());
	}
}

std::string uengine::get_weapon_name(std::uintptr_t current_weapon) {
	std::string weapon_name;

	std::uint64_t weapon_data = kernel->read<std::uint64_t>(current_weapon + offsets->weapon_data);
	std::uint64_t item_name = kernel->read<std::uint64_t>(weapon_data + offsets->item_name);
	if (!item_name)
		return std::string(hash_str(""));

	std::uint64_t f_data = kernel->read<std::uint64_t>(item_name + offsets->data);
	int f_length = kernel->read<int>(item_name + offsets->length);

	if (f_length > 0 && f_length < 50)
	{
		wchar_t* weapon_buffer = new wchar_t[f_length];
		kernel->read_physical((void*)f_data, (void*)weapon_buffer, f_length * sizeof(wchar_t));
		std::wstring w_name(weapon_buffer);
		weapon_name = std::string(w_name.begin(), w_name.end());

		delete[] weapon_buffer;
	}

	return weapon_name;
}

std::string uengine::get_local_weapon_name() {
	std::intptr_t current_weapon = kernel->read<std::uintptr_t>(world_data->acknowledged_pawn + offsets->current_weapon);

	std::string weapon_name;

	std::uint64_t weapon_data = kernel->read<std::uint64_t>(current_weapon + offsets->weapon_data);
	std::uint64_t item_name = kernel->read<std::uint64_t>(weapon_data + offsets->item_name);
	if (!item_name)
		return std::string(hash_str(""));

	std::uint64_t f_data = kernel->read<std::uint64_t>(item_name + offsets->data);
	int f_length = kernel->read<int>(item_name + offsets->length);

	if (f_length > 0 && f_length < 50)
	{
		wchar_t* weapon_buffer = new wchar_t[f_length];
		kernel->read_physical((void*)f_data, (void*)weapon_buffer, f_length * sizeof(wchar_t));
		std::wstring w_name(weapon_buffer);
		weapon_name = std::string(w_name.begin(), w_name.end());

		delete[] weapon_buffer;
	}

	return weapon_name;
}

auto uengine::is_visible(std::uintptr_t mesh) -> bool {
	auto last_submit_time = kernel->read<double>(world_data->u_world + 0x158);
	auto last_render_time = kernel->read<float>(mesh + 0x32c);
	return last_submit_time - last_render_time <= 0.06f;
}