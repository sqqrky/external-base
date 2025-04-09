#ifndef TICK_CPP
#define TICK_CPP

#include <core/includes.hpp>

bool debug_information = true;

auto last_tick = std::chrono::high_resolution_clock::now();
auto timer_stop = std::chrono::high_resolution_clock::now();

std::vector<entity::c_entity> m_list_cache;

void tick::c_tick::get_world() {

	world_data->u_world = kernel->read<std::uintptr_t>(kernel->m_module + offsets->u_world);
	if (!world_data->u_world)
		return;

	if (debug_information)
		std::cout << hash_str(" > world_data->u_world: ") << std::hex << world_data->u_world << std::endl;

	world_data->game_state = kernel->read<std::uintptr_t>(world_data->u_world + offsets->game_state);
	if (!world_data->game_state)
		return;

	if (debug_information)
		std::cout << hash_str(" > world_data->game_state: ") << std::hex << world_data->game_state << std::endl;

	world_data->game_instance = kernel->read<std::uintptr_t>(world_data->u_world + offsets->game_instance);
	if (!world_data->game_instance)
		return;

	if (debug_information)
		std::cout << hash_str(" > world_data->game_instance: ") << std::hex << world_data->game_instance << std::endl;

	world_data->local_players = kernel->read<std::uintptr_t>(kernel->read<std::uintptr_t>(world_data->game_instance + offsets->local_players));
	if (!world_data->local_players)
		return;

	if (debug_information)
		std::cout << hash_str(" > world_data->local_players: ") << std::hex << world_data->local_players << std::endl;

	world_data->player_controller = kernel->read<std::uintptr_t>(world_data->local_players + offsets->player_controller);
	if (!world_data->player_controller)
		return;

	if (debug_information)
		std::cout << hash_str(" > world_data->player_controller: ") << std::hex << world_data->player_controller << std::endl;

	world_data->acknowledged_pawn = kernel->read<std::uintptr_t>(world_data->player_controller + offsets->acknowledged_pawn);
	if (!world_data->acknowledged_pawn)
		return;

	if (debug_information)
		std::cout << hash_str(" > world_data->acknowledged_pawn: ") << std::hex << world_data->acknowledged_pawn << std::endl;

	world_data->view_state = kernel->read<uengine::tarray<std::uintptr_t>>(world_data->local_players + offsets->view_state);
	if (!world_data->view_state.is_valid())
		return;
}

void tick::c_tick::get_actors() {
	const auto actor_array = kernel->read<uengine::tarray<std::uintptr_t>>(world_data->game_state + offsets->player_array).get_itter();

	if (debug_information)
		std::cout << hash_str(" > actor_array.size(): ") << actor_array.size() << std::endl;

	for (auto index = 0ul; index < actor_array.size(); ++index) {
		auto player_state = actor_array[index];
		if (!player_state) continue;

		auto actor = kernel->read<std::uintptr_t>(player_state + offsets->pawn_private);
		if (!actor || actor == world_data->acknowledged_pawn)
			continue;

		auto mesh = kernel->read<std::uintptr_t>(actor + offsets->mesh);
		if (!mesh)
			continue;

		const auto existing_entity = std::find_if(m_list_cache.begin(), m_list_cache.end(),
			[player_state](const entity::c_entity& entity) {
				return entity.player_state == player_state;
			});

		entity::c_entity m_entity;

		if (existing_entity != m_list_cache.end()) {
			m_entity.user_name = existing_entity->user_name;
		}
		else {
			m_entity.user_name = uengine::decrypt_name(player_state);
		}

		m_entity.player_state = player_state;
		m_entity.actor = actor;
		m_entity.mesh = mesh;

		m_list_cache.push_back(std::move(m_entity));
	}

	m_entity_cache.swap(m_list_cache);

	if (!m_list_cache.empty())
		m_list_cache.clear();

	std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

void tick::c_tick::tick() noexcept {
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	for (;;) {

		constexpr auto update_time = 25;

		auto timer_start = std::chrono::high_resolution_clock::now();
		auto count = std::chrono::duration_cast<std::chrono::milliseconds>(
			timer_start - timer_stop
		).count();

		if (count > update_time)
		{
			std::unique_lock<std::mutex> lock(this->m_mutex);
			{
				this->get_world();

				this->get_actors();
			}
			lock.unlock();
			timer_stop = std::move(timer_start);
		}
		else {

			std::this_thread::sleep_for(
				std::chrono::milliseconds(
					update_time - count
				));
		}

	}
}

#endif