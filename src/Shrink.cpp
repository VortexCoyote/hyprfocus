#include "Shrink.hpp"

#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/Window.hpp>
#include <hyprland/src/managers/AnimationManager.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>

void CShrink::init(HANDLE pHandle, std::string animationName) {
    IFocusAnimation::init(pHandle, "shrink");
    addConfigValue(pHandle, "shrink_percentage", SConfigValue{.floatValue = 0.5f});
}

void CShrink::onWindowFocus(CWindow* pWindow, HANDLE pHandle) {
    std::string currentAnimStyle = pWindow->m_vRealSize.getConfig()->internalStyle;
    if ((currentAnimStyle == "popout" || currentAnimStyle == "popin") && pWindow->m_vRealSize.isBeingAnimated())
        return;

    IFocusAnimation::onWindowFocus(pWindow, pHandle);

    const auto SHRINKPERCENTAGE = getConfigValue(pHandle, "shrink_percentage")->floatValue;

    pWindow->m_vRealSize.setConfig(&m_sFocusOutAnimConfig);
    pWindow->m_vRealPosition.setConfig(&m_sFocusOutAnimConfig);
    
    m_sShrinkAnimation.registerVar();
    m_sShrinkAnimation.create(AVARTYPE_FLOAT, 1.0f, &m_sFocusInAnimConfig, pWindow, AVARDAMAGE_ENTIRE);
    m_sShrinkAnimation = SHRINKPERCENTAGE;

    m_sShrinkAnimation.setUpdateCallback([this, pWindow](void* pShrinkAnimation) {
                const auto GOALPOS      = pWindow->m_vRealPosition.goalv();
                const auto GOALSIZE     = pWindow->m_vRealSize.goalv();

                const auto* PANIMATION  = (CAnimatedVariable*)pShrinkAnimation;

                pWindow->m_vRealSize.setValue(GOALSIZE * PANIMATION->fl());
                pWindow->m_vRealPosition.setValue(GOALPOS + GOALSIZE / 2.f - pWindow->m_vRealSize.vec() / 2.f);
            });

    m_sShrinkAnimation.setCallbackOnEnd([this, pWindow](void* pShrinkAnimation) {  
                ((CAnimatedVariable*)pShrinkAnimation)->resetAllCallbacks();
            });
}
