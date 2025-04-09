#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>

namespace world {
	class c_world {
	public:
		std::uintptr_t u_world = 0;
		std::uintptr_t game_instance = 0;
		std::uintptr_t game_state = 0;
		std::uintptr_t local_players = 0;
		std::uintptr_t acknowledged_pawn = 0;
		std::uintptr_t player_controller = 0;
		std::uintptr_t player_array = 0;
		uengine::tarray<std::uintptr_t> view_state;
	};
} inline std::unique_ptr<world::c_world> world_data = std::make_unique<world::c_world>();

#endif