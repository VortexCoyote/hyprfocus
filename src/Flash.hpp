#pragma once

#include "IFocusAnimation.hpp"

class CFlash : public IFocusAnimation {
public:
    void onWindowFocus(CWindow* pWindow, HANDLE pHandle) override; 
    void init(HANDLE pHandle, std::string animationName) override;
};
