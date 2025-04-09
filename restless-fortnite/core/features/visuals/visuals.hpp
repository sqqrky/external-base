#ifndef VISUALS_HPP
#define VISUALS_HPP

#include <mutex>
#include <memory>

namespace visuals {
	class c_visuals {
	private:
		std::mutex m_mutex{};
	public:
		void loop();
		void render();
	};
} inline const auto visual_data = std::make_unique<visuals::c_visuals>();

#endif