#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <d3d11.h>
#include <dependencies/frameworks/imgui/imgui.h>
#include <memory>

// can't name our namespace interface, throws an error due to an already defined structure

namespace n_interface {
    static class c_interface
    {
    public:
        static void set_style();
        static void show();
    };
}  inline const auto interface_data = std::make_unique<n_interface::c_interface>();

inline ImDrawList* draw_list;
inline ImVec2 pos;
inline ImVec2 spacing;
inline ImVec2 region;
inline ImFont* inter_bold;
inline ImFont* inter_default;
inline ImFont* icons;
inline int tabs = 0;

namespace pictures {
    inline ID3D11ShaderResourceView* aaron_tsegay = nullptr;
    inline ID3D11ShaderResourceView* nayte = nullptr;

    // removed, god this code fucking sucks
}

#endif