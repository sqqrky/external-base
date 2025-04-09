#ifndef ENTITY_HPP
#define ENTITY_HPP

namespace entity {
	class c_entity {
	public:
		std::uintptr_t player_state = 0;
		std::uintptr_t actor = 0;
		std::uintptr_t mesh = 0;
		std::string user_name;
	};
} inline std::vector<entity::c_entity> m_entity_cache;

#endif