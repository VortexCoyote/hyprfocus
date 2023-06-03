#pragma once

#include "FocusAnimation.hpp"

#include <hyprland/src/config/ConfigManager.hpp>

class CFlash : public IFocusAnimation {
public:
    virtual void onWindowFocus(CWindow* pWindow, HANDLE pHandle); 
    virtual void init(HANDLE pHandle);

    SAnimationPropertyConfig m_sFlashInAnimConfig;
    SAnimationPropertyConfig m_sFlashOutAnimConfig;
};
