#ifndef MODULE_HPP
#define MODULE_HPP

#include <core/includes.hpp>

namespace module {
	[[nodiscard]] auto get_module(
		device::requests::mba module) -> NTSTATUS;
}

#endif