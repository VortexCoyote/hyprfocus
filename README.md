# hyprfocus

a plugin which provides focus animations for us borderless folks, originally inspired by [flashfocus](https://github.com/fennerm/flashfocus)!

## animations

flash

![preview](flash.gif)

shrink

![preview](shrink.gif)

## compilation & installation

instructions based on [the official wiki](https://wiki.hyprland.org/Plugins/Using-Plugins/#compiling-official-plugins)

**1) clone hyprfocus and cd into it,**

`git clone https://github.com/VortexCoyote/hyprfocus && cd hyprfocus`

**2) clone hyprland and cd into it**

`git clone --recursive https://github.com/hyprwm/Hyprland && cd Hyprland`

**3) checkout the correct release tag matching with your version of hyprland**

`git checkout tags/v0.26.0`

**4) prepare the hyprland source for plugin compilation**

`sudo make pluginenv`

**5) cd back and make**

`cd .. && make all`

**6) profit,** make sure you use the full path to the .so file

`hyprctl plugin load <full .so path>`

**If you compile hyprland manually and install with `sudo make install` you can skip the cloning hyprland sources and `make pluginenv` part.**

## getting started

to start using hyprfocus, add this to your hyprland config:
```
plugin = <full .so path>

plugin:hyprfocus {
    enabled = yes

    keyboard_focus_animation = shrink
    mouse_focus_animation = flash

    bezier = bezIn, 0.5,0.0,1.0,0.5
    bezier = bezOut, 0.0,0.5,0.5,1.0

    flash {
        flash_opacity = 0.7

        in_bezier = bezIn
        in_speed = 0.5

        out_bezier = bezOut
        out_speed = 3
    }

    shrink {
        shrink_percentage = 0.8

        in_bezier = bezIn
        in_speed = 0.5

        out_bezier = bezOut
        out_speed = 3
    }
}
```

### plugin configuration 

`enabled` (yes/no) -> enable or disable the plugin

`keyboard_focus_animation` (flash/shrink/nothing) -> animation for keyboard-driven focus

`mouse_focus_animation` (flash/shrink/nothing) -> animation for mouse pressed focus (setting input:follow_mouse = 0, input:float_switch_override_focus = 0 and input:mouse_refocus = false is recommended)

### animations configuration

animations can be configured within the plugin scope with the following syntax:
```
plugin:hyprfocus {
    <animation> {

    }
}
```

### shared animation variables

`in_bezier` (bezier) -> bezier curve towards the animation apex

`out_bezier` (bezier) -> bezier curve towards the default window state

`in_speed` (float) -> speed for the 'in' bezier

`out_speed` (float) -> speed for the 'out' bezier

### flash

`flash_opacity` (float) -> opacity to during the flash's apex

### shrink

`shrink_percentage` (float) -> the amount a window has shrunk during the animation's apex

### dispatching

hyprfocus can also flash the currently focused window through the `animatefocused` dispatcher:
```
bind = $mod, space, animatefocused
```

## future plans

I'm mostly planning on implementing more animations, for example a popin-popout bounce animation. But if you have any suggestions, feel free to create a git issue!
