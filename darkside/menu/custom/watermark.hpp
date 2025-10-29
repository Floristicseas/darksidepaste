#pragma once
#include <string>
#include "../../darkside.hpp"

class c_watermark_display {
public:
    static int m_pos_x, m_pos_y;
    static bool m_initialized;
    static std::string m_text;
    static float animated_height;

    static void style_setup();
    static void render();
    static void set_text(const std::string& text);
};

inline const auto g_watermark_display = std::make_unique<c_watermark_display>();