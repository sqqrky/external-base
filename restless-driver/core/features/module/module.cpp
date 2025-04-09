#ifndef MODULE_CPP
#define MODULE_CPP

#include <core/features/module/module.hpp>

[[nodiscard]] auto module::get_module(
    device::requests::mba module) -> NTSTATUS {
    if (!module->process_id) {
        return STATUS_UNSUCCESSFUL;
    }

    PEPROCESS process;
    if (!NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)module->process_id, &process))) {
        return STATUS_UNSUCCESSFUL;
    }

    ULONGLONG image_base = *reinterpret_cast <ULONGLONG*> ((uintptr_t)process + 0x520);

    RtlCopyMemory(module->address, &image_base, sizeof(image_base));
    ObDereferenceObject(process);

    return STATUS_SUCCESS;
}

#endif