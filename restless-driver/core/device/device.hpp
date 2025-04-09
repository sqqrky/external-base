#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <core/includes.hpp>

#define code_module CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1C34, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_rwx CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1C35, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_paging CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1C36, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

namespace device {

	namespace requests {
		typedef struct module {
			std::int32_t process_id;
			std::uint64_t* address;
		}mb, * mba;

		typedef struct rwx {
			std::int32_t process_id;
			std::uint64_t address;
			std::uint64_t buffer;
			std::uint64_t size;
			bool write;
		}rw, * rwa;

		typedef struct paging {
			std::int32_t process_id;
			std::uint64_t address;
		}pt, * pta;
	}
}

#endif