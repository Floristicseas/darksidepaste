#include "render.hpp"
#include "../sdk/includes/imgui/imgui_freetype.h"
#define IM_PI                           3.14159265358979323846f

void c_render::update_screen_size(ImGuiIO io) {
	const ImVec2 display_size = io.DisplaySize;

	m_screen_size = vec3_t(display_size.x, display_size.y);

	m_screen_center = m_screen_size * 0.5;

	m_aspect_ratio = m_screen_size.x / m_screen_size.y;
}

void c_render::initialize() {
	auto& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImFontConfig cfg;
	cfg.PixelSnapH = false;
	cfg.OversampleH = 5;
	cfg.OversampleV = 5;

	fonts.verdana = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 13.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	fonts.verdana_small = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	auto prev_flags = cfg.FontBuilderFlags;
	cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;
	fonts.onetap_pixel = io.Fonts->AddFontFromMemoryTTF(onetap_pixel_font, sizeof(onetap_pixel_font), 8.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	fonts.GunIcons = io.Fonts->AddFontFromMemoryTTF(GunIcons, sizeof(GunIcons), 8.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	cfg.FontBuilderFlags = prev_flags;

	io.FontDefault = fonts.verdana;
}

void c_render::update_background_drawlist(ImDrawList* draw_list) {
	m_background_draw_list = draw_list;
}

bool c_render::world_to_screen(vec3_t& in, vec3_t& out) {

	static auto screen_transofrm = reinterpret_cast<bool(__fastcall*)(vec3_t&, vec3_t&)>(g_opcodes->scan(g_modules->m_modules.client_dll.get_name(), "48 89 74 24 ? 57 48 83 EC 20 48 8B 05 ? ? ? ? 48 8B FA"));

	if (!screen_transofrm)
		return false;

	bool on_screen = screen_transofrm(in, out);
	if (on_screen)
		return false;

	const float screen_size_x = m_screen_size.x;
	const float screen_size_y = m_screen_size.y;

	out.x = ((out.x + 1.0f) * 0.5f) * screen_size_x;
	out.y = screen_size_y - (((out.y + 1.0f) * 0.5f) * screen_size_y);

	return true;
}

void c_render::line_legacy(vec3_t start_pos, vec3_t end_pos, c_color color) {
	m_background_draw_list->AddLine(start_pos.im(), end_pos.im(), color.im());
}

void c_render::line(vec3_t start_pos, vec3_t end_pos, c_color color, float thickness) {
	m_background_draw_list->AddLine(start_pos.im(), end_pos.im(), color.im(), thickness);
}

void c_render::line_airflow(float x1, float y1, float x2, float y2, c_color clr, float thickness) {
	m_background_draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), clr.im(), thickness);
}

void c_render::rect(vec3_t start_pos, vec3_t end_pos, c_color color, float rounding) {
	m_background_draw_list->AddRect(start_pos.im(), end_pos.im(), color.im(), rounding);
}

void c_render::rect_outline(vec3_t start_pos, vec3_t end_pos, c_color color, float thickness, float rounding) {
	m_background_draw_list->AddRect(start_pos.im(), end_pos.im(), color.im(), rounding, 0, thickness);
}

void c_render::rect_filled(vec3_t start_pos, vec3_t end_pos, c_color color, float rounding) {
	m_background_draw_list->AddRectFilled(start_pos.im(), end_pos.im(), color.im(), rounding);
}

void c_render::circle(vec3_t center, float radius, c_color color, int segments, float thickness) {
	m_background_draw_list->AddCircle(center.im(), radius, color.im(), segments, thickness);
}

void c_render::circle_filled(vec3_t center, float radius, c_color color, int segments) {
	m_background_draw_list->AddCircleFilled(center.im(), radius, color.im(), segments);
}

void c_render::triangle(vec3_t p1, vec3_t p2, vec3_t p3, c_color color, float thickness) {
	m_background_draw_list->AddTriangle(p1.im(), p2.im(), p3.im(), color.im(), thickness);
}

void c_render::triangle_filled(vec3_t p1, vec3_t p2, vec3_t p3, c_color color) {
	m_background_draw_list->AddTriangleFilled(p1.im(), p2.im(), p3.im(), color.im());
}

void c_render::text(vec3_t txt_pos, c_color color, const int flags, const ImFont* font, const std::string_view& message, const int font_size) {
	m_background_draw_list->PushTextureID(font->ContainerAtlas->TexID);

	const auto size = font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, message.data());
	auto pos = ImVec2(txt_pos.x, txt_pos.y);
	auto outline_clr = c_color(0, 0, 0, color.a * 0.3f);

	if (flags & font_flags_center)
		pos = ImVec2(txt_pos.x - size.x / 2.0f, txt_pos.y);

	if (flags & font_flags_dropshadow)
		m_background_draw_list->AddText(ImVec2(pos.x + 1, pos.y + 1), outline_clr.im(), message.data());

	if (flags & font_flags_outline)
	{
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x + 1, pos.y - 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x - 1, pos.y + 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x - 1, pos.y - 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x + 1, pos.y + 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x, pos.y + 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x, pos.y - 1), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x + 1, pos.y), outline_clr.im(), message.data());
		m_background_draw_list->AddText(font, font_size, ImVec2(pos.x - 1, pos.y), outline_clr.im(), message.data());
	}

	m_background_draw_list->AddText(font, font_size, pos, color.im(), message.data());
}

float c_render::text_width(const ImFont* font, const std::string_view& message, int font_size) {
	return font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, message.data()).x;
}

float c_render::text_height(const ImFont* font, int font_size) {
	return font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, "A").y;
}

void c_render::gradient_horizontal(vec3_t start_pos, vec3_t end_pos, c_color color_left, c_color color_right) {
	m_background_draw_list->AddRectFilledMultiColor(
		start_pos.im(), end_pos.im(),
		color_left.im(), color_right.im(),
		color_right.im(), color_left.im()
	);
}

void c_render::circle_3d(const vec3_t& position, float radius, const c_color& color, int segments) {
	float step = (2.0f * IM_PI) / segments;

	for (int i = 0; i < segments; i++) {
		float angle1 = step * i;
		float angle2 = step * (i + 1);

		vec3_t point1 = position + vec3_t{ cos(angle1) * radius, sin(angle1) * radius, 0 };
		vec3_t point2 = position + vec3_t{ cos(angle2) * radius, sin(angle2) * radius, 0 };

		vec3_t screen1, screen2;
		if (world_to_screen(point1, screen1) && world_to_screen(point2, screen2)) {
			line(screen1, screen2, color);
		}
	}
}

void c_render::gradient_vertical(vec3_t start_pos, vec3_t end_pos, c_color color_top, c_color color_bottom) {
	m_background_draw_list->AddRectFilledMultiColor(
		start_pos.im(), end_pos.im(),
		color_top.im(), color_top.im(),
		color_bottom.im(), color_bottom.im()
	);
}

void c_render::arc(vec3_t center, float radius, float min_angle, float max_angle, c_color color, int segments, float thickness) {
	m_background_draw_list->PathArcTo(center.im(), radius, min_angle, max_angle, segments);
	m_background_draw_list->PathStroke(color.im(), 0, thickness);
}

void c_render::polygon(std::vector<vec3_t> points, c_color color, float thickness) {
	if (points.size() < 2) return;

	m_background_draw_list->PathClear();
	for (auto& point : points)
		m_background_draw_list->PathLineTo(point.im());
	m_background_draw_list->PathStroke(color.im(), true, thickness);
}

void c_render::polygon_filled(std::vector<vec3_t> points, c_color color) {
	if (points.size() < 3) return;

	m_background_draw_list->PathClear();
	for (auto& point : points)
		m_background_draw_list->PathLineTo(point.im());
	m_background_draw_list->PathFillConvex(color.im());
}

void c_render::image(ImTextureID texture, vec3_t pos, vec3_t size, c_color color) {
	m_background_draw_list->AddImage(texture, pos.im(), ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 0), ImVec2(1, 1), color.im());
}

void c_render::image_rounded(ImTextureID texture, vec3_t pos, vec3_t size, c_color color, float rounding) {
	m_background_draw_list->AddImageRounded(texture, pos.im(), ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 0), ImVec2(1, 1), color.im(), rounding);
}

void c_render::progress_bar_horizontal(vec3_t pos, vec3_t size, float progress, c_color background, c_color fill, c_color border, float border_thickness) {
	progress = std::clamp(progress, 0.0f, 1.0f);

	rect_filled(pos, vec3_t(pos.x + size.x, pos.y + size.y), background);

	if (progress > 0.0f) {
		rect_filled(pos, vec3_t(pos.x + size.x * progress, pos.y + size.y), fill);
	}

	if (border_thickness > 0.0f) {
		rect_outline(pos, vec3_t(pos.x + size.x, pos.y + size.y), border, border_thickness);
	}
}

void c_render::progress_bar_vertical(vec3_t pos, vec3_t size, float progress, c_color background, c_color fill, c_color border, float border_thickness) {
	progress = std::clamp(progress, 0.0f, 1.0f);

	rect_filled(pos, vec3_t(pos.x + size.x, pos.y + size.y), background);

	if (progress > 0.0f) {
		float fill_height = size.y * progress;
		rect_filled(vec3_t(pos.x, pos.y + size.y - fill_height), vec3_t(pos.x + size.x, pos.y + size.y), fill);
	}

	if (border_thickness > 0.0f) {
		rect_outline(pos, vec3_t(pos.x + size.x, pos.y + size.y), border, border_thickness);
	}
}

void c_render::health_bar(vec3_t pos, vec3_t size, float health, float max_health, bool vertical) {
	float health_percent = health / max_health;
	c_color health_color = c_color(
		(1.0f - health_percent) * 255,
		health_percent * 255,
		0,
		255
	);

	if (vertical) {
		progress_bar_vertical(pos, size, health_percent, c_color(40, 40, 40, 255), health_color, c_color(0, 0, 0, 255), 1.0f);
	}
	else {
		progress_bar_horizontal(pos, size, health_percent, c_color(40, 40, 40, 255), health_color, c_color(0, 0, 0, 255), 1.0f);
	}
}

void c_render::armor_bar(vec3_t pos, vec3_t size, float armor, float max_armor, bool vertical) {
	float armor_percent = armor / max_armor;
	c_color armor_color = c_color(0, 100, 255, 255);

	if (vertical) {
		progress_bar_vertical(pos, size, armor_percent, c_color(40, 40, 40, 255), armor_color, c_color(0, 0, 0, 255), 1.0f);
	}
	else {
		progress_bar_horizontal(pos, size, armor_percent, c_color(40, 40, 40, 255), armor_color, c_color(0, 0, 0, 255), 1.0f);
	}
}

void c_render::crosshair(vec3_t center, float size, c_color color, float thickness, bool outline) {
	if (outline) {
		line(vec3_t(center.x - size - 1, center.y), vec3_t(center.x + size + 1, center.y), c_color(0, 0, 0, color.a), thickness + 2);
		line(vec3_t(center.x, center.y - size - 1), vec3_t(center.x, center.y + size + 1), c_color(0, 0, 0, color.a), thickness + 2);
	}

	line(vec3_t(center.x - size, center.y), vec3_t(center.x + size, center.y), color, thickness);
	line(vec3_t(center.x, center.y - size), vec3_t(center.x, center.y + size), color, thickness);
}

void c_render::crosshair_circle(vec3_t center, float radius, c_color color, float thickness, int segments) {
	circle(center, radius, color, segments, thickness);
	circle(center, radius * 0.5f, color, segments, thickness);
}

void c_render::box_3d(vec3_t position, vec3_t size, c_color color, float thickness) {
	rect_outline(position, vec3_t(position.x + size.x, position.y + size.y), color, thickness);
}

void c_render::sphere_3d(vec3_t center, float radius, c_color color, int segments, float thickness) {
	circle(center, radius, color, segments, thickness);
}

bool c_render::is_on_screen(vec3_t position, float tolerance) {
	return position.x >= -tolerance && position.x <= m_screen_size.x + tolerance &&
		position.y >= -tolerance && position.y <= m_screen_size.y + tolerance;
}

vec3_t c_render::get_text_size(const ImFont* font, const std::string_view& text, int font_size) {
	auto size = font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, text.data());
	return vec3_t(size.x, size.y);
}

void c_render::blur(vec3_t pos, vec3_t size, float intensity) {
	c_color blur_color = c_color(0, 0, 0, intensity * 50);
	rect_filled(pos, vec3_t(pos.x + size.x, pos.y + size.y), blur_color);
}

void c_render::shadow(vec3_t pos, vec3_t size, float blur, c_color color) {
	for (float i = 0; i < blur; i += 0.5f) {
		float alpha = color.a * (1.0f - i / blur);
		rect_outline(vec3_t(pos.x + i, pos.y + i), vec3_t(pos.x + size.x - i, pos.y + size.y - i), c_color(color.r, color.g, color.b, alpha), 1.0f);
	}
}

void c_render::screen_border(c_color color, float thickness) {
	rect_outline(vec3_t(0, 0), m_screen_size, color, thickness);
}

void c_render::screen_center_marker(c_color color, float size, float thickness) {
	crosshair(m_screen_center, size, color, thickness);
}

void c_render::grid(vec3_t start, vec3_t end, float cell_size, c_color color, float thickness) {
	for (float y = start.y; y <= end.y; y += cell_size) {
		line(vec3_t(start.x, y), vec3_t(end.x, y), color, thickness);
	}

	for (float x = start.x; x <= end.x; x += cell_size) {
		line(vec3_t(x, start.y), vec3_t(x, end.y), color, thickness);
	}
}

void c_render::bezier_curve(vec3_t start, vec3_t control, vec3_t end, c_color color, float thickness, int segments) {
	m_background_draw_list->AddBezierCubic(start.im(), control.im(), control.im(), end.im(), color.im(), thickness, segments);
}


void c_render::gradient(vec3_t start_pos, vec3_t size, c_color col_start, c_color col_end, bool horizontal)
{
	ImVec2 p0 = start_pos.im();
	ImVec2 p1 = ImVec2(start_pos.x + size.x, start_pos.y + size.y);

	if (horizontal)
	{
		m_background_draw_list->AddRectFilledMultiColor(
			p0, p1,
			col_start.im(), col_end.im(),
			col_end.im(), col_start.im()
		);
	}
	else
	{
		m_background_draw_list->AddRectFilledMultiColor(
			p0, p1,
			col_start.im(), col_start.im(),
			col_end.im(), col_end.im()
		);
	}
}

void c_render::arrow(vec3_t start, vec3_t end, c_color color, float thickness, float arrow_size) {
	/*line(start, end, color, thickness);

	vec3_t dir = (end - start).normalized();
	vec3_t perpendicular = vec3_t(-dir.y, dir.x);

	vec3_t arrow1 = end - dir * arrow_size + perpendicular * arrow_size * 0.5f;
	vec3_t arrow2 = end - dir * arrow_size - perpendicular * arrow_size * 0.5f;

	line(end, arrow1, color, thickness);
	line(end, arrow2, color, thickness);*/
}

void c_render::dot(vec3_t position, c_color color, float size) {
	circle_filled(position, size, color);
}

void c_render::window(vec3_t pos, vec3_t size, c_color background, c_color border, float rounding, float border_thickness) {
	rect_filled(pos, vec3_t(pos.x + size.x, pos.y + size.y), background, rounding);
	rect_outline(pos, vec3_t(pos.x + size.x, pos.y + size.y), border, border_thickness, rounding);
}

void c_render::panel(vec3_t pos, vec3_t size, c_color background, c_color border, float rounding, float border_thickness) {
	window(pos, size, background, border, rounding, border_thickness);
}

void c_render::string(float x, float y, c_color clr, int flags, ImFont* font, const std::string& message)
{
	/*auto char_str = message.c_str();

	m_background_draw_list->PushTextureID(font->ContainerAtlas->TexID);

	ImGui::PushFont(font);

	auto coord = ImVec2(x, y);
	auto size = ImGui::CalcTextSize(char_str);
	auto coord_out = ImVec2{ coord.x + 1.f, coord.y + 1.f };
	c_color outline_clr = c_color(0, 0, 0, (flags & outline_light) || (flags & dropshadow_light) ? clr.a() * 0.35f : clr.a());

	int width = 0, height = 0;

	if (!(flags & centered_x))
		size.x = 0;
	if (!(flags & centered_y))
		size.y = 0;

	ImVec2 pos = ImVec2(coord.x - (size.x * .5f), coord.y - (size.y * .5f));

	if (flags & outline_ || flags & outline_light)
	{
		m_background_draw_list->AddText(ImVec2(pos.x + 1, pos.y - 1), outline_clr.as_imcolor(), char_str);
		m_background_draw_list->AddText(ImVec2(pos.x - 1, pos.y + 1), outline_clr.as_imcolor(), char_str);
		m_background_draw_list->AddText(ImVec2(pos.x - 1, pos.y - 1), outline_clr.as_imcolor(), char_str);
		m_background_draw_list->AddText(ImVec2(pos.x + 1, pos.y + 1), outline_clr.as_imcolor(), char_str);

		m_background_draw_list->AddText(ImVec2(pos.x, pos.y + 1), outline_clr.as_imcolor(), char_str);
		m_background_draw_list->AddText(ImVec2(pos.x, pos.y - 1), outline_clr.as_imcolor(), char_str);
		m_background_draw_list->AddText(ImVec2(pos.x + 1, pos.y), outline_clr.as_imcolor(), char_str);
		m_background_draw_list->AddText(ImVec2(pos.x - 1, pos.y), outline_clr.as_imcolor(), char_str);
	}

	if (flags & dropshadow_ || (flags & dropshadow_light))
		m_background_draw_list->AddText(ImVec2(pos.x + 1, pos.y + 1), outline_clr.as_imcolor(), char_str);

	m_background_draw_list->AddText(pos, clr.as_imcolor(), char_str);
	m_background_draw_list->PopTextureID();
	ImGui::PopFont();*/
}