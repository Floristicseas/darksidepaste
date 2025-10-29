#pragma once
#include <string>
#include <vector>
#include <utility>

class c_keybinds_display {
public:
    struct keybind_t {
        std::string name;
        int mode;
    };

    static std::vector<keybind_t> m_keybinds;
    static int m_pos_x, m_pos_y;
    static bool m_initialized;

    static void style_setup();
    static void push(const std::string& name, int mode);
    static void remove(const std::string& name);
    static void render();
    static void reset();
};

inline const auto g_keybinds_display = std::make_unique<c_keybinds_display>();