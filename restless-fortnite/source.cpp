#ifndef SOURCE_CPP
#define SOURCE_CPP

#include <core/includes.hpp>

// depends on the overlay you decide to chuck in here
// but you could js make this a .dll instead of an executable

// also no i don't follow this spacing format throughout 
// troll project dont use this on some shit2c project

auto main( ) -> std::uint32_t {
	if (kernel->attach_km()) {
		std::printf(hash_str(" > yeah, 1c34, all a nigga know, intialized succesfully"));
	}

	kernel->m_pid = kernel->get_process_id( hash_str( L"FortniteClient-Win64-Shipping.exe" ));

	kernel->m_module = kernel->get_image_base( );

	kernel->get_directory_table_base( );

	std::printf( hash_str( " > process_id: %d\n" ), kernel->m_pid );
	std::printf( hash_str( " > module_base: %p\n" ), kernel->m_module );

	// easier formatted compared to cout

	std::jthread( [ & ]( ) -> void { tick_data->tick( ); } ).detach( );

	//overlay_data->initalize();
	//overlay_data->initalize_directx();
	//overlay_data->render_loop();
}

#endif