#pragma once

#define WLR_USE_UNSTABLE

#include <hyprland/src/plugins/PluginAPI.hpp>

class IFocusAnimation {
public:
    // IFocusAnimation() = delete;

    virtual void onWindowFocus(CWindow* pWindow, HANDLE pHandle) = 0;
    virtual void init(HANDLE pHandle) = 0;
};
