#ifndef TICK_HPP
#define TICK_HPP

#include <mutex>

namespace tick
{
	class c_tick
	{
	public:

		std::mutex m_mutex;

		static void get_world();
		static void get_actors();
		void tick() noexcept;
		static void reset() { }
	};
} inline std::unique_ptr<tick::c_tick> tick_data = std::make_unique<tick::c_tick>();

#endif