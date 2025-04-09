#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <core/features/module/module.hpp>
#include <core/features/rwx/rwx.hpp>
#include <core/features/paging/paging.hpp>

#include <core/includes.hpp>

namespace controller {
	auto request_handler(PDEVICE_OBJECT device_object, PIRP irp) -> NTSTATUS;

	auto dispatch_handler(PDEVICE_OBJECT device_object, PIRP irp) -> NTSTATUS;

	auto unsupported_handler(PDEVICE_OBJECT device_object, PIRP irp) -> NTSTATUS;
}

#endif