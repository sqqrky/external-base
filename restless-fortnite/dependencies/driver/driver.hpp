#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <string>
#include <dependencies/driver/requests/requests.hpp>

namespace km
{
    class c_km
    {
    public:
        void* m_handle;
        std::uint32_t m_pid;
        std::uintptr_t m_module;

        auto attach_km( ) -> bool;
        auto get_process_id( const std::wstring& proc_name ) -> std::uintptr_t;
        auto get_image_base( ) -> std::uintptr_t;
        auto get_directory_table_base( ) -> std::uintptr_t;
        auto read_physical( void* addr, void* buffer, DWORD size ) -> bool;
        auto write_physical( void* addr, void* buffer, DWORD size ) -> bool;

        template <typename T>
        T read( std::uint64_t addr ) {
            T buffer{ };
            read_physical(( void* )addr, &buffer, sizeof( T ));
            return buffer;
        }

        template <typename T>
        bool write( std::uint64_t addr, const T& data ) {
            return write_physical( reinterpret_cast< void* >( addr ), ( void* )&data, sizeof( T ));
        }

        auto read_array( std::uintptr_t addr, void* buffer, std::size_t size ) -> void
        {
            if ( size > 0 )
            {
                read_physical(( void* )addr, buffer, size );
            }
        }

        auto read_string( const std::uintptr_t addr, void* buffer, std::size_t size ) -> std::string {
            this->read_array( addr, buffer, size );
            char name[ 255 ] = { 0 };
            std::memcpy( &name, buffer, size );
            return std::string( name );
        }
    }; 
} inline std::unique_ptr< km::c_km > kernel = std::make_unique< km::c_km> () ;

#endif