 #define WLR_USE_UNSTABLE

#include <src/plugins/PluginAPI.hpp>

inline HANDLE PHANDLE = nullptr;

#include <src/Window.hpp>
#include <src/Compositor.hpp>
#include <src/managers/AnimationManager.hpp>

typedef void (*origFocusWindow)(void*, CWindow*, wlr_surface*);

CFunctionHook* g_pFocusWindowHook = nullptr;
CWindow* g_pPreviouslyFocusedWindow = nullptr;

SAnimationPropertyConfig g_FlashInAnimConfig;
SAnimationPropertyConfig g_FlashOutAnimConfig;

void flashWindow(CWindow* pWindow) {
    static auto* const PHYPRFOCUSOPACITY = &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hyprfocus:opacity")->floatValue;
    pWindow->m_fAlpha = *PHYPRFOCUSOPACITY;
    
    static auto* const PFLASHINBEZIER = &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hyprfocus:flash_in_bezier")->strValue;
    static auto* const PFLASHINSPEED  = &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hyprfocus:flash_in_speed")->floatValue;

    g_FlashInAnimConfig.internalBezier = *PFLASHINBEZIER;
    g_FlashInAnimConfig.internalSpeed = *PFLASHINSPEED;
    
    pWindow->m_fAlpha.setConfig(&g_FlashInAnimConfig);

    pWindow->m_fAlpha.setCallbackOnEnd([pWindow](void*) {  
            //Make sure we restore to the active window opacity
            auto* const PACTIVEOPACITY = &HyprlandAPI::getConfigValue(PHANDLE, "decoration:active_opacity")->floatValue;
            pWindow->m_fAlpha = *PACTIVEOPACITY;

            static auto* const PFLASHOUTBEZIER = &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hyprfocus:flash_out_bezier")->strValue;
            static auto* const PFLASHOUTSPEED  = &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hyprfocus:flash_out_speed")->floatValue;

            g_FlashOutAnimConfig.internalBezier = *PFLASHOUTBEZIER;
            g_FlashOutAnimConfig.internalSpeed = *PFLASHOUTSPEED;
            
            pWindow->m_fAlpha.setConfig(&g_FlashOutAnimConfig);
        });
}

void flashCurrentWindow(std::string) {
    static auto* const PHYPRFOCUSENABLED = &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hyprfocus:enabled")->intValue;

    if (!*PHYPRFOCUSENABLED)
        return;

    if(g_pPreviouslyFocusedWindow == nullptr)
        return;

    flashWindow(g_pPreviouslyFocusedWindow);
}

void hkFocusWindow(void* thisptr, CWindow* pWindow, wlr_surface* pSurface) {
    (*(origFocusWindow)g_pFocusWindowHook->m_pOriginal)(thisptr, pWindow, pSurface);

    static auto* const PHYPRFOCUSENABLED = &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hyprfocus:enabled")->intValue;

    if (!*PHYPRFOCUSENABLED)
        return;

    if(pWindow == nullptr)
        return;

    if(pWindow == g_pPreviouslyFocusedWindow)
        return;

    flashWindow(pWindow);
    g_pPreviouslyFocusedWindow = pWindow;
}

// Do NOT change this function.
APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;    

    HyprlandAPI::addConfigValue(PHANDLE, "plugin:hyprfocus:enabled", SConfigValue{.intValue   = 0   });
    HyprlandAPI::addConfigValue(PHANDLE, "plugin:hyprfocus:opacity", SConfigValue{.floatValue = 0.5f});

    HyprlandAPI::addConfigValue(PHANDLE, "plugin:hyprfocus:flash_in_bezier",  SConfigValue{.strValue = "default"});
    HyprlandAPI::addConfigValue(PHANDLE, "plugin:hyprfocus:flash_out_bezier", SConfigValue{.strValue = "default"});

    HyprlandAPI::addConfigValue(PHANDLE, "plugin:hyprfocus:flash_in_speed",  SConfigValue{.floatValue = 1.f});
    HyprlandAPI::addConfigValue(PHANDLE, "plugin:hyprfocus:flash_out_speed", SConfigValue{.floatValue = 5.f});

    HyprlandAPI::reloadConfig();

    HyprlandAPI::addDispatcher(PHANDLE, "flashwindow", &flashCurrentWindow);

    g_FlashInAnimConfig = *(g_pConfigManager->getAnimationPropertyConfig("global"));
    g_FlashInAnimConfig.internalEnabled = 1;
    g_FlashInAnimConfig.internalStyle = "";
    g_FlashInAnimConfig.pValues = &g_FlashInAnimConfig;

    g_FlashOutAnimConfig = *(g_pConfigManager->getAnimationPropertyConfig("global"));
    g_FlashOutAnimConfig.internalEnabled = 1;
    g_FlashOutAnimConfig.internalStyle = "";
    g_FlashOutAnimConfig.pValues = &g_FlashOutAnimConfig;

    g_pFocusWindowHook = HyprlandAPI::createFunctionHook(handle, (void*)&CCompositor::focusWindow, (void*)&hkFocusWindow);
    g_pFocusWindowHook->hook();

    return {"hyprfocus", "Flash windows on focus", "Vortex", "1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
   HyprlandAPI::invokeHyprctlCommand("seterror", "disable");
}
