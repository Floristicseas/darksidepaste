#include "event.hpp"
#include "sounds.hpp"

#include <cassert>
#include <playsoundapi.h>

#include "../../darkside.hpp"
#include "../../features/visuals/world.hpp"

#include "logs/logs.hpp"

#include "tracers.hpp"

#include "../../render/render.hpp"


const char* m_groups[] = {
   xorstr_("body"),
   xorstr_("head"),
   xorstr_("chest"),
   xorstr_("body"),
   xorstr_("left arm"),
   xorstr_("right arm"),
   xorstr_("left leg"),
   xorstr_("right leg"),
   xorstr_("neck"),
   xorstr_("unknown"),
   xorstr_("gear")
};

c_cs_player_controller* c_game_event_helper::GetPlayerController()
{
	if (!Event)
		return nullptr;

	int controller_id{};

	c_buffer buffer;
	buffer.name = xorstr_("userid");

	Event->GetControllerId(controller_id, &buffer);

	if (controller_id == -1)
		return nullptr;

	return (c_cs_player_controller*)g_interfaces->m_entity_system->get_base_entity(controller_id + 1);
}

c_cs_player_controller* c_game_event_helper::GetAttackerController()
{
	if (!Event)
		return nullptr;

	int controller_id{};

	c_buffer buffer;
	buffer.name = xorstr_("attacker");

	Event->GetControllerId(controller_id, &buffer);

	if (controller_id == -1)
		return nullptr;

	return (c_cs_player_controller*)g_interfaces->m_entity_system->get_base_entity(controller_id + 1);
}

int c_game_event_helper::GetDamage()
{
	if (!Event)
		return -1;

	return Event->get_int2(xorstr_("dmg_health"), false);
}

int c_game_event::get_int2(const char* Name, bool Unk)
{
	using GetEventInt_t = int(__fastcall*)(void*, const char*, bool);
	static GetEventInt_t GetEventInt = reinterpret_cast<GetEventInt_t>(g_opcodes->scan(g_modules->m_modules.client_dll.get_name(), xorstr_("48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 41 56 48 83 EC 30 48 8B 01 41 8B F0 4C 8B F1 41 B0 01 48 8D 4C 24 20 48 8B DA 48 8B 78")));

	return GetEventInt(this, Name, Unk);
}

float c_game_event::get_float2(const char* Name, bool Unk)
{
	using GetEventFloat_t = float(__fastcall*)(void*, const char*, bool);
	static GetEventFloat_t GetEventFloat = reinterpret_cast<GetEventFloat_t>(g_opcodes->scan(g_modules->m_modules.client_dll.get_name(), xorstr_("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 40 48 8B 01 48 8B F1 0F 29 74 24 30 48 8D 4C 24 20 0F 28 F2 48 8B DA 48 8B 78")));

	return GetEventFloat(this, Name, Unk);
}

bool c_events::initialize()
{
	g_interfaces->m_game_event_manager->add(this, xorstr_("player_hurt"), false);
	if (!g_interfaces->m_game_event_manager->find(this, xorstr_("player_hurt")))
		return true;

	g_interfaces->m_game_event_manager->add(this, xorstr_("player_death"), false);
	if (!g_interfaces->m_game_event_manager->find(this, xorstr_("player_death")))
		return true;

	g_interfaces->m_game_event_manager->add(this, xorstr_("bullet_impact"), false);
	if (!g_interfaces->m_game_event_manager->find(this, xorstr_("bullet_impact")))
		return true;

	g_interfaces->m_game_event_manager->add(this, xorstr_("round_start"), false);
	if (!g_interfaces->m_game_event_manager->find(this, xorstr_("round_start")))
		return true;

	g_interfaces->m_game_event_manager->add(this, xorstr_("round_freeze_end"), false);
	if (!g_interfaces->m_game_event_manager->find(this, xorstr_("round_freeze_end")))
		return true;

	g_interfaces->m_game_event_manager->add(this, xorstr_("round_announce_warmup"), false);
	if (!g_interfaces->m_game_event_manager->find(this, xorstr_("round_announce_warmup")))
		return true;

	g_interfaces->m_game_event_manager->add(this, xorstr_("round_officially_ended"), false);
	if (!g_interfaces->m_game_event_manager->find(this, xorstr_("round_officially_ended")))
		return true;

	g_interfaces->m_game_event_manager->add(this, xorstr_("round_end"), false);
	if (!g_interfaces->m_game_event_manager->find(this, xorstr_("round_end")))
		return true;

	return false;
}

void c_events::fire_game_event(c_game_event* event)
{
	std::string name = event->get_name();

	if (name.find(xorstr_("bullet_impact")) != std::string::npos)
		on_bullet_tracer(event);

	if (name.find(xorstr_("player_death")) != std::string::npos)
		player_death(event);

	if (name.find(xorstr_("player_hurt")) != std::string::npos)
		on_player_hurt(event);
}

void c_events::player_death(c_game_event* event)
{
	if (!g_interfaces->m_engine->is_connected() || !g_interfaces->m_engine->is_in_game())
		return;

	auto player = event->get_event_helper().GetPlayerController();
	auto attacker = event->get_event_helper().GetAttackerController();

	if (!player || !attacker)
		return;
}

void c_events::draw_hitmarkers()
{
	if (hitmarkers.empty())
		return;

	float currentTime = g_interfaces->m_global_vars->m_real_time;
	float duration = 0.5f; 
	float size = 8.0f;    

	for (auto it = hitmarkers.begin(); it != hitmarkers.end(); )
	{
		float alpha = 1.0f - (currentTime - it->time) / duration;

		if (alpha <= 0.0f)
		{
			it = hitmarkers.erase(it); 
			continue;
		}

		ImU32 color = ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, alpha));
		ImVec2 p = it->pos;

		g_render->line_airflow(p.x - size, p.y - size, p.x + size, p.y + size, color, 1.f);
		g_render->line_airflow(p.x - size, p.y + size, p.x + size, p.y - size, color, 1.f);

		++it;
	}
}

void c_events::on_player_hurt(c_game_event* event)
{
	if (!g_interfaces->m_engine->is_connected() || !g_interfaces->m_engine->is_in_game())
		return;

	if (!g_ctx->m_local_pawn->is_alive())
		return;

	c_game_event_helper EventHelper = event->get_event_helper();

	auto pPlayer = EventHelper.GetPlayerController();
	auto attacker = EventHelper.GetAttackerController();

	if (!pPlayer || !attacker)
		return;

	auto pawn = g_ctx->m_local_pawn;
	if (!pawn)
		return;

	auto localController = g_ctx->m_local_controller;
	if (!localController)
		return;

	auto damage = EventHelper.GetDamage();
	if (damage < 1)
		return;

	auto hitbox = event->get_int2(xorstr_("hitgroup"), false);
	auto dmg = event->get_int2(xorstr_("dmg_health"), false);
	auto health = event->get_int2(xorstr_("health"), false);
	auto name = reinterpret_cast<c_cs_player_controller*>(pPlayer)->m_player_name();

	std::string msg = std::format("Hit {} in the {} for {} damage [{} remaining]",
		name, m_groups[hitbox], dmg, health);

	if (attacker == localController)
	{
		if (g_cfg->misc.m_hitsound)
		{
			if (g_cfg->misc.m_hit_sound_mode == 0)
				PlaySoundA(reinterpret_cast<LPCSTR>(flick_sound), NULL, SND_MEMORY | SND_ASYNC);
			if (g_cfg->misc.m_hit_sound_mode == 1)
				PlaySoundA(reinterpret_cast<LPCSTR>(bell_sound), NULL, SND_MEMORY | SND_ASYNC);
			if (g_cfg->misc.m_hit_sound_mode == 2)
				PlaySoundA(reinterpret_cast<LPCSTR>(mettalic_sound), NULL, SND_MEMORY | SND_ASYNC);
			if (g_cfg->misc.m_hit_sound_mode == 3)
				PlaySoundA(reinterpret_cast<LPCSTR>(bubble_sound), NULL, SND_MEMORY | SND_ASYNC);
		}

		if (g_cfg->misc.m_hit_logs)
		{			
			g_event_logs->push(msg, c_color(1.f, 1.f, 1.f, 1.f), 4.f );
		}

		if (g_cfg->misc.m_hitmark)
		{
			hitmarker_data hm;
			hm.time = g_interfaces->m_global_vars->m_real_time;

			int screen_w = g_render->m_screen_size.x;
			int screen_h = g_render->m_screen_size.y;
			hm.pos = ImVec2(screen_w * 0.5f, screen_h * 0.5f);

			hitmarkers.push_back(hm);
		}

	}
}

void c_events::on_bullet_tracer(c_game_event* event)
{
	if (!g_interfaces->m_engine->is_connected() || !g_interfaces->m_engine->is_in_game())
		return;

	if (!g_ctx->m_local_pawn->is_alive())
		return;

	c_utl_string_token token_x(xorstr_("x"));
	c_utl_string_token token_y(xorstr_("y"));
	c_utl_string_token token_z(xorstr_("z"));

	auto player = event->get_event_helper().GetPlayerController();
	auto position = vec3_t(event->get_float_new2(token_x), event->get_float_new2(token_y), event->get_float_new2(token_z));

	c_cs_player_pawn* local_pawn = g_ctx->m_local_pawn;

	if (!local_pawn)
		return;

	c_game_event_helper event_helper = event->get_event_helper();

	auto pPlayer = event_helper.GetPlayerController();

	if (event->get_event_helper().GetPlayerController() == g_ctx->m_local_controller)
	{
		g_bullet_tracer->add_bullet_trace(local_pawn->get_eye_pos(), position, (g_cfg->world.m_bullet_tracer * 255));

		if (!g_cfg->world.m_hit_effect)
			return;

		g_bullet_tracer->add_hit_effect(position, (255, 255, 255));
	}

}