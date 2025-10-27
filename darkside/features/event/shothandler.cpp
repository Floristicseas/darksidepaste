#include "shothandler.hpp"
#include "../../render/render.hpp"

void CShotHandler::RenderHitMarker(const ImVec2& pos, float size)
{
	auto color = g_cfg->misc.hitmark_clr;
	//auto color = clr.GetU32();

	auto draw = ImGui::GetBackgroundDrawList();
	ImU32 outline = IM_COL32(0, 0, 0, 255);

	//draw->AddLine(ImVec2(pos.x - size, pos.y - size), ImVec2(pos.x - size / 2, pos.y - size / 2), outline, 1.0f);
	//g_render->line_legacy(ImVec2(pos.x - size, pos.y - size), ImVec2(pos.x - size / 2, pos.y - size / 2), color);

	//draw->AddLine(ImVec2(pos.x + size, pos.y - size), ImVec2(pos.x + size / 2, pos.y - size / 2), outline, 1.0f);
	//draw->AddLine(ImVec2(pos.x + size, pos.y - size), ImVec2(pos.x + size / 2, pos.y - size / 2), color);

	//draw->AddLine(ImVec2(pos.x - size, pos.y + size), ImVec2(pos.x - size / 2, pos.y + size / 2), outline, 1.0f);
	//draw->AddLine(ImVec2(pos.x - size, pos.y + size), ImVec2(pos.x - size / 2, pos.y + size / 2), color);

	//draw->AddLine(ImVec2(pos.x + size, pos.y + size), ImVec2(pos.x + size / 2, pos.y + size / 2), outline, 1.0f);
	//draw->AddLine(ImVec2(pos.x + size, pos.y + size), ImVec2(pos.x + size / 2, pos.y + size / 2), color);
}

