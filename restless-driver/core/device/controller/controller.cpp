#ifndef CONTROLLER_CPP
#define CONTROLLER_CPP

#include <core/device/controller/controller.hpp>

auto controller::unsupported_handler(PDEVICE_OBJECT device_object, PIRP irp) -> NTSTATUS
{
	UNREFERENCED_PARAMETER(device_object);

	irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return irp->IoStatus.Status;
}

auto controller::dispatch_handler(PDEVICE_OBJECT device_object, PIRP irp) -> NTSTATUS
{
	UNREFERENCED_PARAMETER(device_object);

	PIO_STACK_LOCATION pio_stack_location = IoGetCurrentIrpStackLocation(irp);

	switch (pio_stack_location->MajorFunction)
	{
	case IRP_MJ_CREATE:
		break;
	case IRP_MJ_CLOSE:
		break;
	default:
		break;
	}

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

auto controller::request_handler(PDEVICE_OBJECT device_object, PIRP irp) -> NTSTATUS
{
	UNREFERENCED_PARAMETER(device_object);
	PIO_STACK_LOCATION pio_stack_location = IoGetCurrentIrpStackLocation(irp);

	NTSTATUS status_t = STATUS_UNSUCCESSFUL;
	ULONG bytes = { };

	ULONG code_input = pio_stack_location->Parameters.DeviceIoControl.IoControlCode;
	ULONG size = pio_stack_location->Parameters.DeviceIoControl.InputBufferLength;

	if (code_input == code_module)
	{
		if (size == sizeof(device::requests::mb))
		{
			device::requests::mba request = (device::requests::mba)(irp->AssociatedIrp.SystemBuffer);
			status_t = module::get_module(request);
			bytes = sizeof(device::requests::mb);
		}
	}

	else if (code_input == code_rwx)
	{
		if (size == sizeof(device::requests::rw))
		{
			device::requests::rwa request = (device::requests::rwa)(irp->AssociatedIrp.SystemBuffer);
			status_t = rwx::process_calls(request);
			bytes = sizeof(device::requests::rw);
		}
	}
	else if (code_input == code_paging)
	{
		device::requests::pta req = (device::requests::pta)(irp->AssociatedIrp.SystemBuffer);

		status_t = STATUS_SUCCESS;
		paging::update::update_cache((HANDLE)req->process_id);
		bytes = sizeof(device::requests::pta);
	}
	else {
		status_t = STATUS_INFO_LENGTH_MISMATCH;
		bytes = (0);
	}

	irp->IoStatus.Status = status_t;
	irp->IoStatus.Information = bytes;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return status_t;
}

#endif