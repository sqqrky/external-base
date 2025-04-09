#ifndef RENDER_CPP
#define RENDER_CPP

#include <dwmapi.h>
#include <D3D11.h>
#include <d3dx11.h>
#include <dependencies/frameworks/imgui/backend/imgui_impl_dx11.h>
#include <dependencies/frameworks/imgui/backend/imgui_impl_win32.h>
#include <dependencies/frameworks/imgui/imgui.h>
#include <dependencies/frameworks/imgui/imgui_internal.h>
#include <core/render/imports/imports.hpp>
#include <core/render/other/fonts/fonts.hpp>
#include <core/render/interface/interface.hpp>
#include <core/includes.hpp>

// honestly one of the most annoying parts, i removed for obvious reasons

bool overlay::c_overlay::initalize_directx() {
	return true;
}

auto overlay::c_overlay::cleanup_directx() -> void {
}

bool overlay::c_overlay::initalize() {
	return true;
}

void overlay::c_overlay::render() {
}

bool overlay::c_overlay::render_loop() {
	return true;
}

#endif