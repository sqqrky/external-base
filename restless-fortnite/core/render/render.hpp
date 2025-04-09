#ifndef RENDER_HPP
#define RENDER_HPP

#include <memory>

namespace overlay {
    class c_overlay
    {
    public:
        static bool initalize();
        static void render();
        static void cleanup_directx();
        static bool render_loop();
        static bool initalize_directx();

    private:
        HWND m_hwnd;
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_device_ctx;
        IDXGISwapChain* m_swap_chain;
        ID3D11RenderTargetView* m_render_target;
    };
} inline std::unique_ptr<overlay::c_overlay> overlay_data = std::make_unique<overlay::c_overlay>();

#endif