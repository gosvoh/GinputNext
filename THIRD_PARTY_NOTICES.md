# Third-party notices

## SDL2

Project: Simple DirectMedia Layer 2  
Pinned release: **2.32.10**  
Upstream: `libsdl-org/SDL`  
License: zlib license.

The project downloads SDL2's official Visual C++ development package at build
time and dynamically links `SDL2.dll`.

## Plugin-SDK

Project: `DK22Pac/plugin-sdk`  
Pinned target commit:
`624a6a49265fd7a6fc63bda1611013ceabeacb8a`

The GTASA workflow compiles the pinned SDK and links its static library. This source tree includes no
Plugin-SDK source or binaries.

The target game-specific libraries are:

- GTA III: `plugin_iii.lib`
- Vice City: `plugin_vc.lib`
- San Andreas: `plugin.lib`

## Controller mappings

SDL_GameControllerDB by the SDL community, revision
`42f28e22d20761e7004e8db91c4ad86402fdf600`, is distributed under the
zlib license. The archive includes the upstream notices in `licenses/`.
