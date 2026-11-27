#pragma once
#include <cstring>
#include <string_view>

namespace gin {
// Text is already localized. Only replace complete SA control tokens; leave
// keyboard-only actions, unknown tokens and all GXT formatting to the game.
inline void FormatDualActionPrompts(char* text, bool modern, int padMode) {
    if (!text || padMode < 0 || padMode > 1) return;
    struct Binding {
        std::string_view action;
        const char* classic;
        const char* modern;
    };
    static constexpr Binding bindings[] = {
        {"PED_ANSWER_PHONE", "L1 (5)", nullptr},
        {"PED_JUMPING", "1", "1"}, {"PED_SPRINT", "2", "2"},
        {"PED_DUCK", "L3 (11)", "L3 (11)"}, {"PED_LOOKBEHIND", "R3 (12)", "R3 (12)"},
        {"PED_LOCK_TARGET", "R1 (6)", "L2 (7)"},
        {"PED_FIREWEAPON", "3", nullptr}, // Modern depends on the equipped weapon.
        {"PED_FIREWEAPON_ALT", "L1 (5)", nullptr},
        {"PED_CYCLE_WEAPON_LEFT", "L2 (7)", "L1 (5)"},
        {"PED_CYCLE_WEAPON_RIGHT", "R2 (8)", "R1 (6)"},
        {"PED_CENTER_CAMERA_BEHIND_PLAYER", "L1 (5)", nullptr},
        {"VEHICLE_ENTER_EXIT", "4", "4"}, {"CAMERA_CHANGE_VIEW_ALL_SITUATIONS", "9", "9"},
        {"VEHICLE_ACCELERATE", "2", nullptr}, // Bicycle differs in Modern.
        {"VEHICLE_BRAKE", "1", "L2 (7)"}, {"VEHICLE_HANDBRAKE", "R1 (6)", nullptr},
        {"VEHICLE_LOOKLEFT", "L2 (7)", nullptr}, {"VEHICLE_LOOKRIGHT", "R2 (8)", nullptr},
        {"VEHICLE_LOOKBEHIND", "L2 + R2", nullptr},
        {"TOGGLE_SUBMISSIONS", "R3 (12)", "R3 (12)"},
        {"VEHICLE_RADIO_STATION_UP", "D-Up", "D-Up"},
        {"VEHICLE_RADIO_STATION_DOWN", "D-Down", "D-Down"},
        {"GROUP_CONTROL_FWD", "D-Up", "D-Up"}, {"GROUP_CONTROL_BWD", "D-Down", "D-Down"},
        {"CONVERSATION_YES", "D-Right", "D-Right"}, {"CONVERSATION_NO", "D-Left", "D-Left"},
        {"GO_FORWARD", "LS Up", "LS Up"}, {"GO_BACK", "LS Down", "LS Down"},
        {"GO_LEFT", "LS Left", "LS Left"}, {"GO_RIGHT", "LS Right", "LS Right"},
        {"VEHICLE_STEERLEFT", "LS Left", "LS Left"}, {"VEHICLE_STEERRIGHT", "LS Right", "LS Right"}
    };
    char* cursor = text;
    while (char* token = std::strstr(cursor, "~k~~")) {
        char* name = token + 4;
        char* end = std::strchr(name, '~');
        if (!end) break;
        const std::string_view action(name, end - name);
        const char* label = nullptr;
        if (action == "VEHICLE_HORN") {
            label = padMode == 0 ? "L3 (11)" : "L1 (5)";
        } else {
            for (const auto& binding : bindings) {
                if (binding.action == action) {
                    label = modern ? binding.modern : binding.classic;
                    break;
                }
            }
        }
        cursor = name;
        if (!label) continue;
        const auto size = std::strlen(label);
        if (size > static_cast<size_t>(end + 1 - token)) continue;
        std::memcpy(token, label, size);
        std::memmove(token + size, end + 1, std::strlen(end + 1) + 1);
        cursor = token + size;
    }
}
}
