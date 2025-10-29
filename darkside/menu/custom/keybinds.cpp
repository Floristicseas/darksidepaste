#include "../../darkside.hpp"
#include "keybinds.hpp"

std::vector<c_keybinds_display::keybind_t> c_keybinds_display::m_keybinds;
int c_keybinds_display::m_pos_x = g_cfg->indicators.keybindposx;
int c_keybinds_display::m_pos_y = g_cfg->indicators.keybindposy;
bool c_keybinds_display::m_initialized = false;
float animated_height = 50.f; 

void c_keybinds_display::style_setup() {
    auto& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.PopupRounding = 0.0f;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
}

void c_keybinds_display::push(const std::string& name, int mode) {
    for (const auto& keybind : m_keybinds)
        if (keybind.name == name) return;
    m_keybinds.push_back({ name, mode });
}

void c_keybinds_display::remove(const std::string& name) {
    m_keybinds.erase(std::remove_if(m_keybinds.begin(), m_keybinds.end(),
        [&](const keybind_t& k) { return k.name == name; }), m_keybinds.end());
}

void c_keybinds_display::render() {
    if (!g_cfg->indicators.m_indicators[0])
        return;

    if (!m_initialized) {
        m_initialized = true;
        if (m_pos_x == 0 && m_pos_y == 0) { m_pos_x = 100; m_pos_y = 100; }
    }

    const float window_width = 220.f;
    const float first_bind_height = 60.f;
    const float per_bind_height = 28.f;
    const float target_height = m_keybinds.empty() ? 50.f : first_bind_height + ((m_keybinds.size() - 1) * per_bind_height);

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

    if (ImGui::Begin("##keybinds", nullptr, flags)) {
        ImVec2 pos = ImGui::GetWindowPos();

        if (menu_open) {
            m_pos_x = (int)pos.x;
            m_pos_y = (int)pos.y;
        }

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 size = ImGui::GetWindowSize();

        draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(18, 18, 25, 230), 12.f);

        draw->AddRectFilledMultiColor(
            pos,
            ImVec2(pos.x + size.x, pos.y + 26.f),
            IM_COL32(0, 180, 255, 255),
            IM_COL32(0, 180, 255, 255),
            IM_COL32(0, 120, 255, 255),
            IM_COL32(0, 120, 255, 255)
        );

        ImGui::SetCursorPosY(4.f);
        ImGui::SetCursorPosX(10.f);
        ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Keybinds");

        float y_offset = 30.f;
        for (const auto& keybind : m_keybinds) {
            ImGui::PushID(keybind.name.c_str());

            ImVec2 line_start = ImVec2(pos.x + 10.f, pos.y + y_offset);
            ImVec2 line_end = ImVec2(pos.x + size.x - 10.f, pos.y + y_offset + 22.f);            

            ImGui::SetCursorPosY(y_offset);
            ImGui::SetCursorPosX(12.f);
            ImGui::Text("%s", keybind.name.c_str());

            const char* mode_text = "";
            ImVec4 mode_color;
            switch (keybind.mode) {
            case 0: mode_text = "Hold";   mode_color = ImVec4(0.2f, 0.8f, 0.3f, 1.f); break;
            case 1: mode_text = "Toggle"; mode_color = ImVec4(0.8f, 0.6f, 0.2f, 1.f); break;
            case 2: mode_text = "Always"; mode_color = ImVec4(0.8f, 0.3f, 0.3f, 1.f); break;
            default: mode_text = "Unknown"; mode_color = ImVec4(0.5f, 0.5f, 0.5f, 1.f); break;
            }

            float text_width = ImGui::CalcTextSize(mode_text).x;
            ImGui::SameLine(size.x - text_width - 12.f);
            ImGui::TextColored(mode_color, "%s", mode_text);

            ImGui::PopID();
            y_offset += per_bind_height;
        }

        ImGui::End();
    }
}
