# hyprfocus

An animation plugin for us borderless folks inspired by [flashfocus](https://github.com/fennerm/flashfocus)!

![preview](hyprfocus.gif)

To use hyprfocus, add the plugin and add this to your config:
```
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


**Variables:**

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
