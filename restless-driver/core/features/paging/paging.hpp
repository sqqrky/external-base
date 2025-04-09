#ifndef PAGING_HPP
#define PAGING_HPP

#include <dependencies/ia32/ia32.hpp>
#include <core/includes.hpp>

namespace paging
{
	[[nodiscard]] auto read_physical_address(
		std::uint64_t address,
		void* buffer,
		std::uint64_t size,
		size_t* bytes) -> bool;

	[[nodiscard]] auto split_memory(
		void* start,
		size_t size,
		const void* pattern
	) -> void*;

	[[nodiscard]] auto get_mm_pfn_database() -> bool;

	[[nodiscard]] auto dirbase_from_handle(
		void* base_address) -> std::uintptr_t;

	namespace update {
		[[nodiscard]] auto update_cache(
			HANDLE process_id) -> void;
	}

	inline void* mm_pfn_database;
}

#endif