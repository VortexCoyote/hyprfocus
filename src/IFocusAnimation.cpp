#include "IFocusAnimation.hpp"

#include <hyprland/src/plugins/PluginAPI.hpp>

void IFocusAnimation::init(HANDLE pHandle, std::string animationName) {
    m_szAnimationName = animationName;

    HyprlandAPI::addConfigValue(pHandle, configPrefix() + "in_bezier",  SConfigValue{.strValue = "default"});
    HyprlandAPI::addConfigValue(pHandle, configPrefix() + "out_bezier", SConfigValue{.strValue = "default"});

    HyprlandAPI::addConfigValue(pHandle, configPrefix() + "in_speed",  SConfigValue{.floatValue = 1.f});
    HyprlandAPI::addConfigValue(pHandle, configPrefix() + "out_speed", SConfigValue{.floatValue = 5.f});

    m_sFocusInAnimConfig                    = *(g_pConfigManager->getAnimationPropertyConfig("global"));
    m_sFocusInAnimConfig.internalEnabled    = 1;
    m_sFocusInAnimConfig.internalStyle      = std::string("hyprfocus_") + animationName + std::string("_in");
    m_sFocusInAnimConfig.pValues            = &m_sFocusInAnimConfig;

    m_sFocusOutAnimConfig                   = *(g_pConfigManager->getAnimationPropertyConfig("global"));
    m_sFocusOutAnimConfig.internalEnabled   = 1;
    m_sFocusOutAnimConfig.internalStyle     = std::string("hyprfocus_") + animationName + std::string("_out");
    m_sFocusOutAnimConfig.pValues           = &m_sFocusOutAnimConfig;
}

void IFocusAnimation::onWindowFocus(CWindow* pWindow, HANDLE pHandle) {
    m_sFocusInAnimConfig.internalBezier       = HyprlandAPI::getConfigValue(pHandle, configPrefix() + "in_bezier")->strValue;
    m_sFocusInAnimConfig.internalSpeed        = HyprlandAPI::getConfigValue(pHandle, configPrefix() + "in_speed")->floatValue;

    m_sFocusOutAnimConfig.internalBezier      = HyprlandAPI::getConfigValue(pHandle, configPrefix() + "out_bezier")->strValue;
    m_sFocusOutAnimConfig.internalSpeed       = HyprlandAPI::getConfigValue(pHandle, configPrefix() + "out_speed")->floatValue;
}

void IFocusAnimation::addConfigValue(HANDLE pHandle, std::string name, SConfigValue sValue) {
    HyprlandAPI::addConfigValue(pHandle, configPrefix() + name, sValue);
}

SConfigValue* IFocusAnimation::getConfigValue(HANDLE pHandle, std::string name) {
    return HyprlandAPI::getConfigValue(pHandle, configPrefix() + name);
}
