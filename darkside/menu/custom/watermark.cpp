#include "watermark.hpp"

int c_watermark_display::m_pos_x = g_cfg->indicators.watermarkposx;
int c_watermark_display::m_pos_y = g_cfg->indicators.watermarkposy;
bool c_watermark_display::m_initialized = false;
std::string c_watermark_display::m_text = "darkside";
float c_watermark_display::animated_height = 50.f;

void c_watermark_display::style_setup() {
    auto& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.PopupRounding = 0.0f;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
}

void c_watermark_display::set_text(const std::string& text) {
    m_text = text;
}

void c_watermark_display::render() {
    if (!g_cfg->indicators.m_indicators[1])
        return;

    if (!m_initialized) {
        m_initialized = true;
        if (m_pos_x == 0 && m_pos_y == 0) { m_pos_x = 100; m_pos_y = 100; }
    }

    const float window_width = 150.f;
    const float target_height = 50.f;

    animated_height += (target_height - animated_height) * 0.12f;

    ImGui::SetNextWindowSize(ImVec2(window_width, animated_height), ImGuiCond_Always);

    bool menu_open = g_menu->m_opened;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBackground;
    if (!menu_open)
        flags |= ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos(ImVec2((float)m_pos_x, (float)m_pos_y), ImGuiCond_Once);

    style_setup();

    if (ImGui::Begin("##watermark", nullptr, flags)) {
        ImVec2 pos = ImGui::GetWindowPos();

        if (menu_open) {
            m_pos_x = (int)pos.x;
            m_pos_y = (int)pos.y;
        }

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 size = ImGui::GetWindowSize();

       //draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(18, 18, 25, 230), 12.f);

        draw->AddRectFilledMultiColor(
            pos,
            ImVec2(pos.x + size.x, pos.y + 26.f),
            IM_COL32(0, 180, 255, 255),
            IM_COL32(0, 180, 255, 255),
            IM_COL32(0, 120, 255, 255),
            IM_COL32(0, 120, 255, 255)
        );

        ImGui::SetCursorPosY(4.f);
        auto text_size = ImGui::CalcTextSize(m_text.c_str());
        ImGui::SetCursorPosX((size.x - text_size.x) * 0.5f);
        ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "%s", m_text.c_str());

        ImGui::SetCursorPosY(30.f);
        ImGui::SetCursorPosX(12.f);

        ImGui::End();
    }
}