# mpv-debug-plugin

a debug tool for mpv, based on the [C PLUGINS](https://mpv.io/manual/master/#c-plugins) API.

![screenshot](screenshot/debug.jpg)

## Features

- Visual view of mpv's internal properties
- Console with completion, history support
- Colorful mpv logs view with filter support

## Installation

[mpv](https://mpv.io) >= `0.37.0` is required, and the `cplugins` feature should be enabled.

Download the plugin from Releases, and place `debug.dll` in your mpv `scripts` folder.

## Configuration

> **NOTE:** If you changed the dll name, `script-message-to` target should apply too.

Add a keybinding to show debug window (required):

**input.conf**
```
` script-message-to debug show
```
