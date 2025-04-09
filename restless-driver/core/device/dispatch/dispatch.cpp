#ifndef DISPATCH_CPP
#define DISPATCH_CPP

#include <core/device/dispatch/dispatch.hpp>

auto dispatch::initalize_driver(PDRIVER_OBJECT driver_object, PUNICODE_STRING device_path) -> NTSTATUS
{
	UNREFERENCED_PARAMETER(device_path);

	NTSTATUS status = STATUS_UNSUCCESSFUL;
	PDEVICE_OBJECT device_object = { nullptr };

	UNICODE_STRING unicode_name, unicode_link;
	wchar_t device_name[260];
	wchar_t link_name[260];
	wchar_t random_string[128];

	int id[4];
	__cpuid(id, 1);
	wchar_t buffer[1];

	StringCchPrintfW(random_string, sizeof(random_string), L"%08X", id[0]);

	StringCchPrintfW(device_name, sizeof(device_name), L"\\Device\\%s", random_string);
	StringCchPrintfW(link_name, sizeof(link_name), L"\\DosDevices\\%s", random_string);

	RtlInitUnicodeString(&unicode_name, device_name);
	RtlInitUnicodeString(&unicode_link, link_name);

	status = IoCreateDevice(driver_object, 0, &unicode_name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &device_object);

	if (!NT_SUCCESS(status))
		return status;
	status = IoCreateSymbolicLink(&unicode_link, &unicode_name);

	if (!NT_SUCCESS(status))
		return status;

	for (int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
		driver_object->MajorFunction[i] = &controller::unsupported_handler;

	device_object->Flags |= DO_BUFFERED_IO;

	driver_object->MajorFunction[IRP_MJ_CREATE] = &controller::dispatch_handler;
	driver_object->MajorFunction[IRP_MJ_CLOSE] = &controller::dispatch_handler;
	driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = &controller::request_handler;

	device_object->Flags &= ~DO_DEVICE_INITIALIZING;

	return status;
}

auto dispatch::create() -> NTSTATUS
{
	return IoCreateDriver(nullptr, &initalize_driver);
}

#endif