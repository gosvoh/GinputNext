// Exercise the production SDL path without GTA or a physical controller.
#define SDL_MAIN_HANDLED
#include "ControllerCore.h"
#include "DualActionPrompts.h"
#include <array>
#include <cassert>
#include <cmath>
#include <cstdio>

int main(int argc, char** argv) {
    assert(argc == 2);
    gin::ControllerCore core;
    gin::Config config;
    config.allowGenericDirectInput = false;
    config.backgroundInput = true;
    config.hotplugScanFrames = 1;
    assert(core.Init(config, argv[1], argv[1]));

    // SDL assigns a backend-specific GUID to virtual devices. The production
    // VID/PID fallback must install the mapping before opening the controller.
    SDL_VirtualJoystickDesc device{};
    device.version = SDL_VIRTUAL_JOYSTICK_DESC_VERSION;
    device.vendor_id = 0x046D;
    device.product_id = 0xC216;
    device.name = "Virtual Dual Action";
    device.nbuttons = 12;
    device.naxes = 4;
    device.nhats = 1;
    const int index = SDL_JoystickAttachVirtualEx(&device);
    assert(index >= 0);
    auto* raw = SDL_JoystickOpen(index);
    assert(raw);
    core.Tick();
    assert(core.IsConnected());
    assert(core.IsMappedDualAction());
    char message[] = "~y~~k~~PED_ANSWER_PHONE~~s~ / ~k~~UNKNOWN~ / ~k~~PED_SPRINT~";
    gin::FormatDualActionPrompts(message, false, 0);
    assert(std::strcmp(message, "~y~L1 (5)~s~ / ~k~~UNKNOWN~ / 2") == 0);
    char modern[] = "~k~~PED_ANSWER_PHONE~ / ~k~~PED_LOCK_TARGET~";
    gin::FormatDualActionPrompts(modern, true, 0);
    assert(std::strcmp(modern, "~k~~PED_ANSWER_PHONE~ / L2 (7)") == 0);
    const unsigned buttons[] = {GIN_BTN_X, GIN_BTN_A, GIN_BTN_B, GIN_BTN_Y,
        GIN_BTN_LB, GIN_BTN_RB, 0, 0, GIN_BTN_BACK, GIN_BTN_START, GIN_BTN_L3, GIN_BTN_R3};
    for (int i = 0; i < 12; ++i) {
        for (int pressed : {1, 0}) {
            assert(SDL_JoystickSetVirtualButton(raw, i, pressed) == 0);
            core.Tick();
            const auto& s = core.State();
            assert(gin::ButtonMask(s) == (pressed ? buttons[i] : 0));
            assert(s.leftTrigger == (i == 6 ? pressed : 0));
            assert(s.rightTrigger == (i == 7 ? pressed : 0));
        }
    }
    for (int hat : {0, 1, 3, 2, 6, 4, 12, 8, 9, 0}) {
        assert(SDL_JoystickSetVirtualHat(raw, 0, hat) == 0);
        core.Tick();
        const auto& s = core.State();
        assert(s.dpadUp == bool(hat & 1) && s.dpadRight == bool(hat & 2));
        assert(s.dpadDown == bool(hat & 4) && s.dpadLeft == bool(hat & 8));
    }
    for (int axis = 0; axis < 4; ++axis) {
        for (int value : {-32768, -16384, 0, 16384, 32767, 0}) {
            assert(SDL_JoystickSetVirtualAxis(raw, axis, value) == 0);
            core.Tick();
            const auto& s = core.State();
            const std::array<float, 4> values = {s.leftX, s.leftY, s.rightX, s.rightY};
            for (int i = 0; i < 4; ++i) {
                if (i != axis || value == 0) assert(values[i] == 0);
                else if (value == -32768 || value == 32767) assert(values[i] == (value < 0 ? -1 : 1));
                else assert(values[i] * (value < 0 ? -1 : 1) > 0.2f && std::abs(values[i]) < 0.8f);
            }
        }
    }
    SDL_JoystickClose(raw);
    assert(SDL_JoystickDetachVirtual(index) == 0);
    core.Tick();
    assert(!core.IsConnected());
    assert(!core.IsMappedDualAction());
    core.Shutdown();
    std::puts("Virtual Dual Action passed through ControllerCore.");
}
