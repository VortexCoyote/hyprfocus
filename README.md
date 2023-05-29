# hyprfocus

An animation plugin for us borderless folks inspired by [flashfocus](https://github.com/fennerm/flashfocus)!

![preview](hyprfocus.gif)

## compilation & installation

instructions based on [the official wiki](https://wiki.hyprland.org/Plugins/Using-Plugins/#compiling-official-plugins)

**1) clone hyprfocus and cd into it,**

`git clone https://github.com/VortexCoyote/hyprfocus && cd hyprfocus`

**2) clone hyprland and cd into it**

`git clone --recursive https://github.com/hyprwm/Hyprland && cd Hyprland`

**3) checkout the correct release tag matching with your version of hyprland**

`git checkout tags/v0.25.0`

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
hyprctl plugin load <full .so path>

plugin:hyprfocus {
    enabled = yes
    opacity = 0.7

    bezier = flashIn, 0.5,0.0,1.0,0.5
    bezier = flashOut, 0.0,0.5,0.5,1.0

    flash_in_bezier = flashIn
    flash_in_speed = 1

    flash_out_bezier = flashOut
    flash_out_speed = 5
}
```

### variables

`enabled` (yes/no) -> Enable or disable the plugin

`opacity` (float) -> Opacity to during the flash's apex

`flash_in_bezier` (bezier) -> Bezier curve for flash towards the opacity apex

`flash_out_bezier` (bezier) -> Bezier curve for the flash towards default window opacity

`flash_in_speed` (float) -> Towards opacity apex animation speed

`flash_out_speed` (float) -> Towards default window opacity animation speed


hyprfocus can also flash the currently focused window through the `flashwindow` dispatcher:
```
bind = $mod, space, flashwindow
```

## future plans

I'm mostly planning on implementing more animations, for example a popin-popout bounce animation. But if you have any suggestions, feel free to create a git issue!
