 #define WLR_USE_UNSTABLE

#include <hyprland/src/plugins/PluginAPI.hpp>

inline HANDLE PHANDLE = nullptr;

#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/Window.hpp>
#include <hyprland/src/managers/AnimationManager.hpp>

#include "Flash.hpp"
#include "Shrink.hpp"

#include <iostream>

CWindow*    g_pPreviouslyFocusedWindow  = nullptr;
bool        g_bMouseWasPressed          = false;

std::unordered_map<std::string, std::unique_ptr<IFocusAnimation>> g_mAnimations;

void flashWindow(CWindow* pWindow) {
    auto* const PTYPE = g_bMouseWasPressed  ? &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hyprfocus:mouse_focus_animation")->strValue
                                            : &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hyprfocus:keyboard_focus_animation")->strValue;

    g_mAnimations[*PTYPE]->onWindowFocus(pWindow, PHANDLE);
}

void flashCurrentWindow(std::string) {
    static auto* const PHYPRFOCUSENABLED = &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hyprfocus:enabled")->intValue;

    if (!*PHYPRFOCUSENABLED)
        return;

    if(g_pPreviouslyFocusedWindow == nullptr)
        return;

    flashWindow(g_pPreviouslyFocusedWindow);
}

static void onActiveWindowChange(void* self, std::any data) {
    try {
        auto* const PWINDOW = std::any_cast<CWindow*>(data);
        static auto* const PHYPRFOCUSENABLED = &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hyprfocus:enabled")->intValue;

        if (!*PHYPRFOCUSENABLED)
            return;

        if(PWINDOW == nullptr)
            return;

        if(PWINDOW == g_pPreviouslyFocusedWindow)
            return;

        flashWindow(PWINDOW);
        g_pPreviouslyFocusedWindow = PWINDOW;

    } catch (std::bad_any_cast& e) { }
}

static void onMouseButton(void* self, std::any data) {
    try {
        auto* const PWLRMOUSEBUTTONEVENT = std::any_cast<wlr_pointer_button_event*>(data);
        g_bMouseWasPressed = PWLRMOUSEBUTTONEVENT->state == WLR_BUTTON_PRESSED;

    } catch (std::bad_any_cast& e) { }
}

// Do NOT change this function.
APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;    

    HyprlandAPI::addConfigValue(PHANDLE, "plugin:hyprfocus:enabled",                    SConfigValue{.intValue = 0      });
    HyprlandAPI::addConfigValue(PHANDLE, "plugin:hyprfocus:keyboard_focus_animation",   SConfigValue{.strValue = "flash"});
    HyprlandAPI::addConfigValue(PHANDLE, "plugin:hyprfocus:mouse_focus_animation",      SConfigValue{.strValue = "flash"});
    
    HyprlandAPI::addDispatcher(PHANDLE, "animatefocused", &flashCurrentWindow);

    g_mAnimations["flash"   ] = std::make_unique<CFlash>();
    g_mAnimations["shrink"  ] = std::make_unique<CShrink>();
    g_mAnimations["nothing" ] = std::make_unique<IFocusAnimation>();

    for(auto& [name, pAnimation] : g_mAnimations) {
        pAnimation->init(PHANDLE, name);
    }

    HyprlandAPI::reloadConfig();

    HyprlandAPI::registerCallbackDynamic(PHANDLE, "activeWindow",    [&](void* self, std::any data) { onActiveWindowChange(self, data); });
    HyprlandAPI::registerCallbackDynamic(PHANDLE, "mouseButton",     [&](void* self, std::any data) { onMouseButton       (self, data); });

    return {"hyprfocus", "animate windows on focus", "Vortex", "2.0"};
}

APICALL EXPORT void PLUGIN_EXIT() { }
