#define NOMINMAX
#include "logs.hpp"
#include "../../../darkside.hpp"
#include "../../../render/render.hpp"
#include <algorithm>

void c_event_logs::push(const std::string& text, c_color color, float duration, int type) {
    log_message_t msg;
    msg.text = text;
    msg.color = color;
    msg.creation_time = ImGui::GetTime();
    msg.duration = duration;
    msg.type = type;

    m_messages.push_back(msg);

    if (m_messages.size() > 20) {
        m_messages.erase(m_messages.begin());
    }
}

void c_event_logs::render() {
    if (m_messages.empty())
        return;

    float current_time = ImGui::GetTime();
    vec3_t current_pos = m_position;

    m_messages.erase(
        std::remove_if(m_messages.begin(), m_messages.end(),
            [&](const log_message_t& msg) {
                return current_time - msg.creation_time > msg.duration + m_fade_time;
            }),
        m_messages.end()
    );

    auto ease_out_cubic = [](float t) { return 1 - powf(1 - t, 3); };
    auto ease_in_cubic = [](float t) { return t * t * t; };
    auto ease_out_bounce = [](float t) {
        const float n1 = 7.5625f, d1 = 2.75f;
        if (t < 1 / d1) return n1 * t * t;
        else if (t < 2 / d1) return n1 * (t -= 1.5f / d1) * t + 0.75f;
        else if (t < 2.5 / d1) return n1 * (t -= 2.25f / d1) * t + 0.9375f;
        else return n1 * (t -= 2.625f / d1) * t + 0.984375f;
        };

    for (auto it = m_messages.rbegin(); it != m_messages.rend(); ++it) {
        const auto& msg = *it;
        float time_alive = current_time - msg.creation_time;

        float alpha = 1.0f;
        float slide_offset = 0.0f;
        float scale = 1.0f;
        float y_drift = 0.0f;

        if (time_alive < m_fade_time) {
            float t = time_alive / m_fade_time;
            float eased = ease_out_cubic(t);
            alpha = eased;
            slide_offset = (1.0f - eased) * 100.0f;
            scale = 0.8f + 0.2f * ease_out_bounce(t);
            y_drift = (1.0f - eased) * 10.0f;
        }
        else if (time_alive > msg.duration) {
            float t = (time_alive - msg.duration) / m_fade_time;
            float eased = ease_in_cubic(t);
            alpha = 1.0f - eased;
            slide_offset = eased * 60.0f;
            scale = 1.0f - 0.15f * t;
            y_drift = eased * 5.0f;
        }

        float text_width = g_render->text_width(g_render->fonts.verdana, msg.text, 16);
        float text_height = g_render->text_height(g_render->fonts.verdana, 16);
        float message_width = std::min(text_width + 60.0f, m_max_width);
        vec3_t message_size = vec3_t(message_width * scale, (text_height + 16) * scale, 0);

        c_color bg_color(0, 0, 0, (int)(alpha * 220));
        c_color text_color = msg.color;
        text_color.a = (int)(alpha * 255);

        vec3_t draw_pos = current_pos + vec3_t(-slide_offset, y_drift, 0);

        g_render->text(draw_pos + vec3_t(35, 10, 0), text_color, 0,
            g_render->fonts.verdana, msg.text, 16);

        current_pos.y += message_size.y + m_spacing;
        if (current_pos.y > g_render->m_screen_size.y - 50)
            break;
    }
}

void c_event_logs::clear() {
    m_messages.clear();
}