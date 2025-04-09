#ifndef SOURCE_CPP
#define SOURCE_CPP

#include <core/device/dispatch/dispatch.hpp>

extern "C" auto main(PDRIVER_OBJECT driver_object, PUNICODE_STRING device_path) -> NTSTATUS
{
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(device_path);

	// there's nothing in here worth taking
	// it's just organized with some shittily written functions and zero protection

	return dispatch::create();
}

#endif