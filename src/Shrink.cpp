#include "Shrink.hpp"

#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/Window.hpp>
#include <hyprland/src/managers/AnimationManager.hpp>

void CShrink::init(HANDLE pHandle) {
    m_sFlashInAnimConfig = *(g_pConfigManager->getAnimationPropertyConfig("global"));
    m_sFlashInAnimConfig.internalEnabled = 1;
    m_sFlashInAnimConfig.internalStyle = "";
    m_sFlashInAnimConfig.pValues = &m_sFlashInAnimConfig;

    m_sFlashOutAnimConfig = *(g_pConfigManager->getAnimationPropertyConfig("global"));
    m_sFlashOutAnimConfig.internalEnabled = 1;
    m_sFlashOutAnimConfig.internalStyle = "";
    m_sFlashOutAnimConfig.pValues = &m_sFlashOutAnimConfig;

    HyprlandAPI::addConfigValue(pHandle, "plugin:hyprfocus:shrink_percentage", SConfigValue{.floatValue = 0.5f});
}

void CShrink::onWindowFocus(CWindow* pWindow, HANDLE pHandle) {

    // if (pWindow->m_vRealPosition.isBeingAnimated() | pWindow->m_vRealSize.isBeingAnimated())
    //     return;

    const auto SHRINKPERCENTAGE = HyprlandAPI::getConfigValue(pHandle, "plugin:hyprfocus:shrink_percentage")->floatValue;

    static auto* const ANIMATEOUTBEZ = &HyprlandAPI::getConfigValue(pHandle, "plugin:hyprfocus:animate_out_bezier")->strValue;
    static auto* const ANIMATEOUTSPEED = &HyprlandAPI::getConfigValue(pHandle, "plugin:hyprfocus:animate_out_speed")->floatValue;

    m_sFlashOutAnimConfig.internalBezier = *ANIMATEOUTBEZ;
    m_sFlashOutAnimConfig.internalSpeed = *ANIMATEOUTSPEED;
         
    pWindow->m_vRealSize.setConfig(&m_sFlashOutAnimConfig);
    pWindow->m_vRealPosition.setConfig(&m_sFlashOutAnimConfig);

    static auto* const ANIMATEINBEZ = &HyprlandAPI::getConfigValue(pHandle, "plugin:hyprfocus:animate_in_bezier")->strValue;
    static auto* const ANIMATEINSPEED = &HyprlandAPI::getConfigValue(pHandle, "plugin:hyprfocus:animate_in_speed")->floatValue;

    m_sFlashInAnimConfig.internalBezier = *ANIMATEINBEZ;
    m_sFlashInAnimConfig.internalSpeed = *ANIMATEINSPEED;

    m_sShrinkAnimation.registerVar();
    m_sShrinkAnimation.create(AVARTYPE_FLOAT, 1.0f, &m_sFlashInAnimConfig, pWindow, AVARDAMAGE_ENTIRE);
    m_sShrinkAnimation = SHRINKPERCENTAGE;

    m_sShrinkAnimation.setUpdateCallback([this, pWindow](void* pShrinkAnimation) {
                const auto GOALPOS  = pWindow->m_vRealPosition.goalv();
                const auto GOALSIZE = pWindow->m_vRealSize.goalv();

                pWindow->m_vRealSize.setValue(GOALSIZE * ((CAnimatedVariable*)pShrinkAnimation)->fl());
                pWindow->m_vRealPosition.setValue(GOALPOS + GOALSIZE / 2.f - pWindow->m_vRealSize.vec() / 2.f);
            });

    m_sShrinkAnimation.setCallbackOnEnd([this, pWindow](void* pShrinkAnimation) {  
                ((CAnimatedVariable*)pShrinkAnimation)->resetAllCallbacks();
            });

}
