#ifndef PAGING_CPP
#define PAGING_CPP

#include <core/features/paging/paging.hpp>

// god this code fucking sucks never use thsi in your life please

[[nodiscard]] auto paging::read_physical_address(
	std::uint64_t address,
	void* buffer,
	std::uint64_t size,
	size_t* bytes) -> bool {

	MM_COPY_ADDRESS physical_address = { 0 };

	return NT_SUCCESS(MmCopyMemory(buffer, physical_address, size, MM_COPY_MEMORY_PHYSICAL, bytes));
}

[[nodiscard]] auto paging::split_memory(
	void* start,
	size_t size,
	const void* pattern
) -> void* {
	auto module_start = static_cast<const unsigned char*>(start);
	auto pattern_start = static_cast<const unsigned char*>(pattern);

	for (std::uintptr_t idx = (0); idx < size - sizeof(pattern); ++idx)
	{
		size_t i = (0);
		for (; i < sizeof(pattern); ++i) {
			if (module_start[idx + i] != pattern_start[i])
				break;
		}

		if (i == sizeof(pattern)) return const_cast<unsigned char*>(&module_start[idx]);
	}

	return (nullptr);
}

[[nodiscard]] auto paging::get_mm_pfn_database(
) -> bool {
	unsigned char shellcode[] = {
					0x48, 0x8B, 0xC1,
					0x48, 0xC1, 0xE8, 0x0C,
					0x48, 0x8D, 0x14, 0x40,
					0x48, 0x03, 0xD2,
					0x48, 0xB8,
	};

	auto mm_get_virtual_for_physical =
		reinterpret_cast<UCHAR*>(MmGetVirtualForPhysical);

	if (!mm_get_virtual_for_physical) {
		return 0;
	}

	auto* function = reinterpret_cast <UCHAR*> (
		split_memory(mm_get_virtual_for_physical, 0x20, shellcode));
	if (!function) {
		return 0;
	}

	mm_pfn_database = PAGE_ALIGN(
		*reinterpret_cast<ULONG_PTR*>(
			function + sizeof(shellcode)));
}

[[nodiscard]] auto paging::dirbase_from_handle(
	void* base_address
) -> uintptr_t {

	virt_addr_t virt_base{ };
	virt_base.value = base_address;

	size_t read{ };
	PPHYSICAL_MEMORY_RANGE physical_ranges = MmGetPhysicalMemoryRanges();
	for (int i = (0);; i++) {
		auto physical_range = &physical_ranges[i];
		if (!physical_range->BaseAddress.QuadPart || !physical_range->NumberOfBytes.QuadPart)
			break;

		uintptr_t current_phys_address = physical_range->BaseAddress.QuadPart;
		for (int j = (0); j < (physical_range->NumberOfBytes.QuadPart / 0x1000); j++, current_phys_address += 0x1000) {
			auto pfn_info = reinterpret_cast <_MMPFN*>(
				reinterpret_cast <uintptr_t>(mm_pfn_database) + (current_phys_address >> 12) * sizeof(_MMPFN));

			if (pfn_info->u4.PteFrame == (current_phys_address >> 12)) {
				MMPTE pml4e{};
				if (!NT_SUCCESS(read_physical_address((current_phys_address + 8 * virt_base.pml4_index),
					&pml4e, 8, &read))) {
					continue;
				}

				if (!pml4e.u.Hard.Valid)
					continue;

				MMPTE pdpte{ };
				if (!NT_SUCCESS(read_physical_address(((pml4e.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pdpt_index),
					&pdpte, 8, &read))) {
					continue;
				}

				if (!pdpte.u.Hard.Valid) {
					continue;
				}

				MMPTE pde{ };
				if (!NT_SUCCESS(read_physical_address(((pdpte.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pd_index),
					&pde, 8, &read))) {
					continue;
				}

				if (!pde.u.Hard.Valid) {
					continue;
				}

				MMPTE pte{ };
				if (!NT_SUCCESS(read_physical_address(((pde.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pt_index),
					&pte, 8, &read))) {
					continue;
				}

				if (!pte.u.Hard.Valid) {
					continue;
				}

				return current_phys_address;
			}
		}
	}

	return 0;
}

[[nodiscard]] auto paging::update::update_cache(
	HANDLE process_id) -> void {

	size_t bytes{ };
	PEPROCESS process{ };
	PsLookupProcessByProcessId(process_id, &process);
	m_dtb = dirbase_from_handle(*(void**)((uintptr_t)process + 0x520));

	for (int i = 0; i < 512; i++) {
		read_physical_address((m_dtb + 8 * i), &m_pml4_table[i], 8, &bytes);
	}

	ObDereferenceObject(process);
}

#endif