#pragma once

#include <memory>

class c_menu {
	int m_selected_tab{ };
public:
	bool m_opened;

	void draw();
	void on_create_move();

	void draw_bg();
	void style_setup();
};

inline const auto g_menu = std::make_unique<c_menu>();