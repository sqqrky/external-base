#ifndef DRIVER_CPP
#define DRIVER_CPP

#include <core/includes.hpp>

auto km::c_km::attach_km( ) -> bool {
	int id[ 4 ];
	__cpuid( id, 1 );
	wchar_t w_buffer[ 1 ];

	wsprintfW( w_buffer, hash_str( L"\\\\.\\\%08X" ), id[ 0 ] );

	this->m_handle = CreateFileW( w_buffer, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );

	return this->m_handle && this->m_handle != INVALID_HANDLE_VALUE;
}

auto km::c_km::get_process_id( const std::wstring& proc_name ) -> std::uintptr_t {
	PROCESSENTRY32 proc_info;
	proc_info.dwSize = sizeof( proc_info );

	HANDLE proc_snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
	if ( proc_snapshot == INVALID_HANDLE_VALUE ) {
		return 0;
	}

	Process32First( proc_snapshot, &proc_info );
	if ( !wcscmp( proc_info.szExeFile, proc_name.c_str( ))) {
		CloseHandle( proc_snapshot );
		return proc_info.th32ProcessID;
	}

	while ( Process32Next( proc_snapshot, &proc_info )) {
		if ( !wcscmp( proc_info.szExeFile, proc_name.c_str( ))) {
			CloseHandle( proc_snapshot );
			return proc_info.th32ProcessID;
		}
	}

	CloseHandle( proc_snapshot );
	return 0;
}

auto km::c_km::get_image_base( ) -> std::uintptr_t {
	std::uintptr_t module_base = { NULL };
	requests::mb arguments = { NULL };
	arguments.process_id = this->m_pid;
	arguments.address = reinterpret_cast <ULONGLONG*> ( &module_base );

	DeviceIoControl( this->m_handle, code_module, &arguments, sizeof( arguments ), nullptr, NULL, NULL, NULL );
	return module_base;
}

auto km::c_km::get_directory_table_base( ) -> std::uintptr_t {
	requests::pt arguments = { NULL };
	arguments.process_id = this->m_pid;

	return DeviceIoControl( this->m_handle, code_paging, &arguments, sizeof( arguments ), nullptr, NULL, NULL, NULL );
}

auto km::c_km::read_physical( void* address, void* buffer, DWORD size ) -> bool {

	requests::rw arguments = { 0 };

	arguments.address = ( ULONGLONG )address;
	arguments.buffer = ( ULONGLONG )buffer;
	arguments.size = size;
	arguments.process_id = this->m_pid;

	return DeviceIoControl( this->m_handle, code_rwx, &arguments, sizeof( arguments ), nullptr, 0, nullptr, nullptr );
}

auto km::c_km::write_physical( void* address, void* buffer, DWORD size ) -> bool
{
	requests::rw arguments = { 0 };

	arguments.address = ( ULONGLONG )address;
	arguments.buffer = ( ULONGLONG )buffer;
	arguments.size = size;
	arguments.process_id = this->m_pid;
	arguments.write = true;

	return DeviceIoControl( this->m_handle, code_rwx, &arguments, sizeof (arguments ), nullptr, NULL, NULL, NULL );
}

#endif