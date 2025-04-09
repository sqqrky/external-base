#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <cstdint>
#include <memory>

struct offset {
public:

	// u_world : public u_object
	static constexpr std::uintptr_t u_world = 0x16eb5008;

	static constexpr std::uintptr_t game_state = 0x198;

	static constexpr std::uintptr_t game_instance = 0x210;

	// u_game_instance : public u_object
	static constexpr std::uintptr_t local_players = 0x38;

	// u_localplayer : public u_object
	static constexpr std::uintptr_t view_state = 0xd0;

	// class a_player_controller : public a_controller
	static constexpr std::uintptr_t player_controller = 0x30;

	static constexpr std::uintptr_t acknowledged_pawn = 0x350;

	static constexpr std::uintptr_t player_state = 0x2c8;

	static constexpr std::uintptr_t targeted_fort_pawn = 0x1980;

	// a_character : public a_pawn
	static constexpr std::uintptr_t root_component = 0x1b0;

	static constexpr std::uintptr_t component_velocity = 0x180;

	static constexpr std::uintptr_t current_weapon = 0xa80;

	static constexpr std::uintptr_t weapon_data = 0x588;

	static constexpr std::uintptr_t ammo_count = 0xf34;

	// u_skeletal_mesh_component : public u_skinned_mesh_component
	static constexpr std::uintptr_t mesh = 0x328;

	static constexpr std::uintptr_t bone_space_transforms = 0x5c8;

	static constexpr std::uintptr_t is_cached = 0x5b8;

	static constexpr std::uintptr_t component_to_world = 0x1e0;

	static constexpr std::uintptr_t get_bounds = 0xe8;

	// a_player_state : public u_object
	static constexpr std::uintptr_t pawn_private = 0x320;

	static constexpr std::uintptr_t team_index = 0x1259;

	static constexpr std::uintptr_t b_is_dying = 0x720;

	static constexpr std::uintptr_t b_is_dbno = 0x962;

	// a_game_state_base : public u_object
	static constexpr std::uintptr_t server_world_time = 0x2e0;

	static constexpr std::uintptr_t player_array = 0x2c0;

	// redacted : dump yourself
	static constexpr std::uintptr_t item_name = 0x40;

	static constexpr std::uintptr_t data = 0x20;

	static constexpr std::uintptr_t length = 0x28;

	static constexpr std::uintptr_t projectile_speed = 0x1d9c;

	static constexpr std::uintptr_t projectile_gravity_scale = 0x1e0c;

}; inline std::unique_ptr<offset> offsets = std::make_unique<offset>();

#endif