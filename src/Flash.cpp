#include "Flash.hpp"

#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/Window.hpp>
#include <hyprland/src/managers/AnimationManager.hpp>

void CFlash::init(HANDLE pHandle) {
    m_sFlashInAnimConfig = *(g_pConfigManager->getAnimationPropertyConfig("global"));
    m_sFlashInAnimConfig.internalEnabled = 1;
    m_sFlashInAnimConfig.internalStyle = "";
    m_sFlashInAnimConfig.pValues = &m_sFlashInAnimConfig;

    m_sFlashOutAnimConfig = *(g_pConfigManager->getAnimationPropertyConfig("global"));
    m_sFlashOutAnimConfig.internalEnabled = 1;
    m_sFlashOutAnimConfig.internalStyle = "";
    m_sFlashOutAnimConfig.pValues = &m_sFlashOutAnimConfig;

    HyprlandAPI::addConfigValue(pHandle, "plugin:hyprfocus:flash_opacity", SConfigValue{.floatValue = 0.5f});
}

void CFlash::onWindowFocus(CWindow* pWindow, HANDLE pHandle) {
    static auto* const PHYPRFOCUSOPACITY = &HyprlandAPI::getConfigValue(pHandle, "plugin:hyprfocus:flash_opacity")->floatValue;
    pWindow->m_fAlpha = *PHYPRFOCUSOPACITY;
    
    static auto* const PFLASHINBEZIER = &HyprlandAPI::getConfigValue(pHandle, "plugin:hyprfocus:animate_in_bezier")->strValue;
    static auto* const PFLASHINSPEED  = &HyprlandAPI::getConfigValue(pHandle, "plugin:hyprfocus:animate_in_speed")->floatValue;

    m_sFlashInAnimConfig.internalBezier = *PFLASHINBEZIER;
    m_sFlashInAnimConfig.internalSpeed = *PFLASHINSPEED;
    
    pWindow->m_fAlpha.setConfig(&m_sFlashInAnimConfig);

    pWindow->m_fAlpha.setCallbackOnEnd([this, pWindow, pHandle](void*) {  
            //Make sure we restore to the active window opacity
            auto* const PACTIVEOPACITY = &HyprlandAPI::getConfigValue(pHandle, "decoration:active_opacity")->floatValue;
            pWindow->m_fAlpha = *PACTIVEOPACITY;

            static auto* const PFLASHOUTBEZIER = &HyprlandAPI::getConfigValue(pHandle, "plugin:hyprfocus:animate_out_bezier")->strValue;
            static auto* const PFLASHOUTSPEED  = &HyprlandAPI::getConfigValue(pHandle, "plugin:hyprfocus:animate_out_speed")->floatValue;

            m_sFlashOutAnimConfig.internalBezier = *PFLASHOUTBEZIER;
            m_sFlashOutAnimConfig.internalSpeed = *PFLASHOUTSPEED;
            
            pWindow->m_fAlpha.setConfig(&m_sFlashOutAnimConfig);
        });
}
