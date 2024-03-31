# accent-color-getter

This is a very simple Node.js native module for use on Windows that gets the
user's selected
[accent
color](https://learn.microsoft.com/en-us/windows/apps/design/signature-experiences/color#accent-color)
and the three light and three dark
[variations](https://learn.microsoft.com/en-us/windows/apps/design/style/color#accent-color-palette)
of the color.

This was conceived for use in [Electron](https://www.electronjs.org/) apps
because (at least currently), the built-in
[`systemPreferences`](https://www.electronjs.org/docs/latest/api/system-preferences#systempreferencesgetcolorcolor-windows-macos)
API only lets you get the system color and not the variations. (Note that the
[`accent-color-changed`](https://www.electronjs.org/docs/latest/api/system-preferences#event-accent-color-changed-windows))
event does work and is useful along with this.)

After a little digging, it seems that calculating the variations is non-trivial[^1],
so just retrieving them from the OS is the best way to match what other apps
would be doing.

# Use

Generally use the `getSystem()` function to get the current system accent
colors. This returns an object with keys `AccentDark3`, `AccentDark2`, etc. Each
of these is an object with the `R`, `G`, `B`, `A` values and some strings for
use in CSS directly.

There are also a few preset groups which may be useful for testing and development.

```js
const AccentColorGetter = require('accent-color-getter');
const getter = new AccentColorGetter();
const systemAccentColors = getter().getSystem();
const navyBlueAccents = getter.presets.navyBlue;
```

Returned objects look like:

```js
{
  // ...

  AccentDark2: {
    id: 3,
    R: 131,
    G: 0,
    B: 72,
    A: 255,
    hexRGB: '#830048',
    hexRGBA: '#830048ff',
    rgba: 'rgba(131, 0, 72, 1.0)'
  },

  // ...
}
```

## Development

This was bootstrapped with the handy
[napi-module](https://www.npmjs.com/package/generator-napi-module)
[Yeoman](https://yeoman.io/) generator. Then the code simply makes appropriate
calls to
[`UISettings.GetColorValue`](https://learn.microsoft.com/en-us/uwp/api/windows.ui.viewmanagement.uisettings.getcolorvalue)
and packages the results.

On macOS, this will install but will simply throw an exception that the
consuming application will need to handle.

[^1]:
    Non-trivial (and undocumented) for precise calculation, but in broad
    strokes, you convert the base RGB value to HSV and then as you go lighter,
    the hue stays roughly constant, the saturation decreases quickly, and the
    value increases relatively slowly; and as you go darker, the hue and
    saturation stay roughly constant, and the value decreases.
