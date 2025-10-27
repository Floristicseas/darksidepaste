#pragma once
#include <string>
#include <vector>
#include <chrono>
#include "../../../darkside.hpp"

struct log_message_t {
	std::string text;
	c_color color;
	float creation_time;
	float duration;
	int type;
	std::string icon;
};

class c_event_logs {
private:
	std::vector<log_message_t> m_messages;
	float m_fade_time = 0.5f;
	vec3_t m_position = vec3_t(10, 10, 0);
	float m_max_width = 300.0f;
	float m_spacing = 5.0f;

public:
	void push(const std::string& text, c_color color = c_color(255, 255, 255, 255), float duration = 5.0f, int type = 0);
	void render();
	void clear();
};

inline const auto g_event_logs = std::make_unique<c_event_logs>();