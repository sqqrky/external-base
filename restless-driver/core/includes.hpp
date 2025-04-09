#ifndef INCLUDES_HPP
#define INCLUDES_HPP

// windows api
#include <ntifs.h>
#include <windef.h>
#include <strsafe.h>

// cpp standard
#include <cstdint>

// msvc compliant
#include <intrin.h>

//proj headers
#include <core/device/device.hpp>

inline std::uint64_t m_dtb;
inline std::uint32_t m_free_pml4_index = -1;
inline std::uint64_t m_pml4_table[512]{ 0 };
inline std::uint64_t m_pdpt_table[512]{ 0 };
inline std::uint64_t m_pd_2mb_table[512][512]{ 0 };

extern "C" NTSTATUS NTAPI IoCreateDriver(PUNICODE_STRING driver_name, PDRIVER_INITIALIZE initialize);

#endif