#ifndef RWX_HPP
#define RWX_HPP

#include <dependencies/imports/imports.hpp>
#include <core/includes.hpp>

namespace rwx
{
	[[nodiscard]] auto read_physical_address(
		std::uint64_t address,
		void* buffer,
		std::uint64_t size,
		size_t* bytes) -> bool;;

	[[nodiscard]] auto write_physical_address(
		std::uint64_t address,
		void* buffer,
		std::uint64_t size,
		size_t* bytes) -> bool;

	[[nodiscard]] auto translate_linear_address(
		std::uint64_t cr3,
		std::uint64_t virtual_address) -> std::uint64_t;

	auto process_calls(
		device::requests::rwa x
	) -> NTSTATUS;
}

#endif