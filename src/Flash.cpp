#include "Flash.hpp"

#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/Window.hpp>
#include <hyprland/src/managers/AnimationManager.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>

void CFlash::init(HANDLE pHandle, std::string animationName) {
    IFocusAnimation::init(pHandle, animationName);
    addConfigValue(pHandle, "flash_opacity", SConfigValue{.floatValue = 0.5f});
}

void CFlash::onWindowFocus(CWindow* pWindow, HANDLE pHandle) {
    IFocusAnimation::onWindowFocus(pWindow, pHandle);

    pWindow->m_fAlpha = getConfigValue(pHandle, "flash_opacity")->floatValue;
    pWindow->m_fAlpha.setConfig(&m_sFocusInAnimConfig);
    pWindow->m_fAlpha.setCallbackOnEnd([this, pWindow, pHandle](void*) {  
            //Make sure we restore to the active window opacity
            pWindow->m_fAlpha = HyprlandAPI::getConfigValue(pHandle, "decoration:active_opacity")->floatValue;
            pWindow->m_fAlpha.setConfig(&m_sFocusOutAnimConfig);
        });
}
