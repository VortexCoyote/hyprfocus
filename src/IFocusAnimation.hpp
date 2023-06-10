#pragma once

#define WLR_USE_UNSTABLE

#include <hyprland/src/config/ConfigManager.hpp>

class IFocusAnimation {
public:
    virtual void onWindowFocus(CWindow* pWindow, HANDLE pHandle);
    virtual void init         (HANDLE pHandle, std::string animationName);

    void            addConfigValue(HANDLE pHandle, std::string name, SConfigValue sValue);
    SConfigValue*   getConfigValue(HANDLE pHandle, std::string name);

public:
    SAnimationPropertyConfig m_sFocusInAnimConfig;
    SAnimationPropertyConfig m_sFocusOutAnimConfig;

    std::string m_szAnimationName;

    std::string configPrefix() { return std::string("plugin:hyprfocus:") + m_szAnimationName + ":"; }
};
