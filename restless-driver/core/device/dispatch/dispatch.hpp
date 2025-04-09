#ifndef DISPATCH_HPP
#define DISPATCH_HPP

#include <core/device/controller/controller.hpp>

namespace dispatch {
	auto initalize_driver(PDRIVER_OBJECT driver_object, PUNICODE_STRING device_path) -> NTSTATUS;

	auto create() -> NTSTATUS;

	extern UNICODE_STRING unicode_name, unicode_link;

	extern wchar_t device_name[260];
	extern wchar_t link_name[260];
	extern wchar_t random_string[128];
}

#endif