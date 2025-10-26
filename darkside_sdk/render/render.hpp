#pragma once

#include "../darkside.hpp"
#include "../sdk/typedefs/c_color.hpp"

#include "bytes.hpp"

enum e_font_flags
{
	font_flags_center = 1 << 0,
	font_flags_outline = 1 << 1,
	font_flags_dropshadow = 1 << 2
};

enum class gradient_type
{
	horizontal,
	vertical
};

struct c_fonts
{
public:
	ImFont* verdana;
	ImFont* verdana_small;
	ImFont* onetap_pixel;
	ImFont* GunIcons;
};

class c_render {
public:
	vec3_t m_screen_size;
	vec3_t m_screen_center;
	float m_aspect_ratio;
	ImDrawList* m_background_draw_list;

	c_fonts fonts;

	void circle_3d(const vec3_t& position, float radius, const c_color& color, int segments = 12);

	void update_screen_size(ImGuiIO);

	void initialize();

	void update_background_drawlist(ImDrawList*);

	bool world_to_screen(vec3_t&, vec3_t&);

	void line_legacy(vec3_t start_pos, vec3_t end_pos, c_color color);
	void line(vec3_t start_pos, vec3_t end_pos, c_color color, float thickness = 1.0f);
	void rect(vec3_t start_pos, vec3_t end_pos, c_color color, float rounding = 0.0f);
	void rect_outline(vec3_t start_pos, vec3_t end_pos, c_color color, float thickness = 1.f, float rounding = 0.0f);
	void rect_filled(vec3_t start_pos, vec3_t end_pos, c_color color, float rounding = 0.0f);

	void circle(vec3_t center, float radius, c_color color, int segments = 12, float thickness = 1.0f);
	void circle_filled(vec3_t center, float radius, c_color color, int segments = 12);

	void triangle(vec3_t p1, vec3_t p2, vec3_t p3, c_color color, float thickness = 1.0f);
	void triangle_filled(vec3_t p1, vec3_t p2, vec3_t p3, c_color color);

	void text(vec3_t txt_pos, c_color color, const int flags, const ImFont* font, const std::string_view& message, const int font_size);
	float text_width(const ImFont* font, const std::string_view& message, int font_size);
	float text_height(const ImFont* font, int font_size);

	void gradient_horizontal(vec3_t start_pos, vec3_t end_pos, c_color color_left, c_color color_right);
	void gradient_vertical(vec3_t start_pos, vec3_t end_pos, c_color color_top, c_color color_bottom);

	void arc(vec3_t center, float radius, float min_angle, float max_angle, c_color color, int segments = 12, float thickness = 1.0f);
	void polygon(std::vector<vec3_t> points, c_color color, float thickness = 1.0f);
	void polygon_filled(std::vector<vec3_t> points, c_color color);

	void image(ImTextureID texture, vec3_t pos, vec3_t size, c_color color = c_color(255, 255, 255, 255));
	void image_rounded(ImTextureID texture, vec3_t pos, vec3_t size, c_color color, float rounding);

	void progress_bar_horizontal(vec3_t pos, vec3_t size, float progress, c_color background, c_color fill, c_color border = c_color(0, 0, 0, 0), float border_thickness = 0.0f);
	void progress_bar_vertical(vec3_t pos, vec3_t size, float progress, c_color background, c_color fill, c_color border = c_color(0, 0, 0, 0), float border_thickness = 0.0f);

	void health_bar(vec3_t pos, vec3_t size, float health, float max_health, bool vertical = false);
	void armor_bar(vec3_t pos, vec3_t size, float armor, float max_armor, bool vertical = false);

	void crosshair(vec3_t center, float size, c_color color, float thickness = 1.0f, bool outline = true);
	void crosshair_circle(vec3_t center, float radius, c_color color, float thickness = 1.0f, int segments = 12);

	void box_3d(vec3_t position, vec3_t size, c_color color, float thickness = 1.0f);
	void sphere_3d(vec3_t center, float radius, c_color color, int segments = 12, float thickness = 1.0f);

	bool is_on_screen(vec3_t position, float tolerance = 0.0f);
	vec3_t get_text_size(const ImFont* font, const std::string_view& text, int font_size);

	void blur(vec3_t pos, vec3_t size, float intensity);
	void shadow(vec3_t pos, vec3_t size, float blur, c_color color = c_color(0, 0, 0, 100));

	void screen_border(c_color color, float thickness = 1.0f);
	void screen_center_marker(c_color color, float size = 5.0f, float thickness = 1.0f);

	void grid(vec3_t start, vec3_t end, float cell_size, c_color color, float thickness = 1.0f);

	void bezier_curve(vec3_t start, vec3_t control, vec3_t end, c_color color, float thickness = 1.0f, int segments = 20);

	void arrow(vec3_t start, vec3_t end, c_color color, float thickness = 1.0f, float arrow_size = 5.0f);

	void gradient(vec3_t start_pos, vec3_t size, c_color col_start, c_color col_end, bool horizontal);

	void dot(vec3_t position, c_color color, float size = 2.0f);

	void window(vec3_t pos, vec3_t size, c_color background, c_color border, float rounding = 0.0f, float border_thickness = 1.0f);
	void panel(vec3_t pos, vec3_t size, c_color background, c_color border, float rounding = 0.0f, float border_thickness = 1.0f);
};

inline const auto g_render = std::make_unique<c_render>();