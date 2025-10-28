#include "tracers.hpp"

//pasted from priority src

void c_particle_snapshot::draw(int count, void* data) {
	vmt::call_virtual< void >(this, 1U, count, data);
}

void c_particle_manager::create_snapshot(c_strong_handle< c_particle_snapshot >* out_particle_snapshot)
{
	__int64 nUnknown = 0;
	vmt::call_virtual< void >(this, 42, out_particle_snapshot, &nUnknown);
}

void c_particle_manager::draw(c_strong_handle<c_particle_snapshot>* particle_snapshot, int count, void* data) {
	vmt::call_virtual< void >(this, 43, particle_snapshot, count, data);
}

void c_game_particle_manager_system::create_effect_index(unsigned int* effect_index, particle_effect* effect_data)
{
	using fn_t = void(__fastcall*)(c_game_particle_manager_system*, unsigned int*, particle_effect*);
	static fn_t fn = reinterpret_cast<fn_t>(g_opcodes->scan(g_modules->m_modules.client_dll.get_name(), xorstr_("40 57 48 83 EC 20 49 8B ?? 48 8B")));
	fn(this, effect_index, effect_data);
}

void c_game_particle_manager_system::set_effect(unsigned int effect_index, int unk, void* clr, int unk2)
{
	using fn_t = void(__fastcall*)(c_game_particle_manager_system*, unsigned int, int, void*, int);
	static fn_t fn = reinterpret_cast<fn_t>(g_opcodes->scan(g_modules->m_modules.client_dll.get_name(), xorstr_("48 89 5C 24 ?? 48 89 74 24 10 57 48 83 EC ?? ?? ?? ?? ?? ?? ?? ?? ?? 41 8B F8 8B DA 4C")));
	fn(this, effect_index, unk, clr, unk2);
}

void c_game_particle_manager_system::fn_init_effect(int effect_index, unsigned int unk, const c_strong_handle< c_particle_snapshot >* particle_snapshot)
{
	using fn_t = bool(__fastcall*)(c_game_particle_manager_system*, int, unsigned int, const c_strong_handle< c_particle_snapshot >*);
	static fn_t fn = reinterpret_cast<fn_t>(g_opcodes->scan(g_modules->m_modules.client_dll.get_name(), xorstr_("48 89 74 24 10 57 48 83 EC 30 4C 8B D9 49 8B F9 33 C9 41 8B F0 83 FA FF 0F")));
	fn(this, effect_index, unk, particle_snapshot);
}

std::vector< tracer_info > bullets{};

void cbullet_tracer::add_bullet_trace(vec3_t start, vec3_t end, c_color clr_)
{
	if (!g_cfg->world.m_bullet_tracer)
		return;

	if (!g_interfaces->m_game_particle_manager_system)
	{
		using fn_t = c_game_particle_manager_system * (__fastcall*)();
		static fn_t fn = reinterpret_cast<fn_t>(g_opcodes->scan(g_modules->m_modules.client_dll.get_name(), xorstr_("48 8B 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC 48 89 5c 24 ? 48 89 74 24 ? 57 48 83 EC 40 49 8B F8")));
		g_interfaces->m_game_particle_manager_system = fn();
	}

	auto& bullet = bullets.emplace_back();

	particle_effect particle_effect_{};

	if (g_cfg->world.m_hit_effect_mode == 0) // Line
		particle_effect_.name = xorstr_("particles/entity/spectator_utility_trail.vpcf");

	if (g_cfg->world.m_hit_effect_mode == 1)
		particle_effect_.name = xorstr_("particles/entity/spectator_utility_trail.vpcf");

	particle_effect_.pad[0] = 8;

	g_interfaces->m_game_particle_manager_system->create_effect_index(&bullet.effect_index, &particle_effect_);
	particle_color clr = { float(clr_.r), float(clr_.g), float(clr_.b) };
	g_interfaces->m_game_particle_manager_system->set_effect(bullet.effect_index, 16, &clr, 0);

	bullet.particle_data_ = {};

	auto dir = (end - start);
	auto stage_2 = start + (dir * 0.3f);
	auto stage_3 = start + (dir * 0.5f);

	vec3_t positions_[] = { start,stage_2, stage_3, end };

	for (int i{}; i < sizeof(positions_) / sizeof(vec3_t); i++)
	{
		particle_information particle_info{};
		particle_info.time = 4.f;
		particle_info.width = 1.f;
		particle_info.unk2 = 1.f;

		g_interfaces->m_game_particle_manager_system->set_effect(bullet.effect_index, 3, &particle_info, 0);

		bullet.positions = new vec3_t[i + 1];
		bullet.times = new float[i + 1];

		for (int j{}; j < i + 1; j++)
		{
			bullet.positions[j] = positions_[j];
			bullet.times[j] = TICK_INTERVAL * float(j);
		}

		bullet.particle_data_.positions = bullet.positions;
		bullet.particle_data_.times2 = bullet.times;

		g_interfaces->m_particle_manager->create_snapshot(&bullet.handle_snapshot_particle);
		g_interfaces->m_game_particle_manager_system->fn_init_effect(bullet.effect_index, 0, &bullet.handle_snapshot_particle);
		g_interfaces->m_particle_manager->draw(&bullet.handle_snapshot_particle, i + 1, &bullet.particle_data_);

		delete[] bullet.positions;
		delete[] bullet.times;
	}
}

void cbullet_tracer::add_hit_effect(vec3_t start, c_color clr)
{
	if (!g_interfaces->m_game_particle_manager_system)
	{
		using fn_t = c_game_particle_manager_system * (__fastcall*)();
		static fn_t fn = reinterpret_cast<fn_t>(g_opcodes->scan(g_modules->m_modules.client_dll.get_name(), xorstr_("48 8b 05 ? ? ? ? c3 cc cc cc cc cc cc cc cc 48 89 5c 24 ? 57")));
		g_interfaces->m_game_particle_manager_system = fn();
	}

	auto& bullet = bullets.emplace_back();

	particle_effect particle_effect_{};

	if (g_cfg->world.m_hit_effect_mode == 0) // fire chicken
		particle_effect_.name = xorstr_("particles/critters/chicken/chicken_gone_feathers_fire.vpcf");

	if (g_cfg->world.m_hit_effect_mode == 1) // sparks
		particle_effect_.name = xorstr_("particles/explosions_fx/bumpmine_detonate_sparks_core.vpcf");

	if (g_cfg->world.m_hit_effect_mode == 2) //explosion default
		particle_effect_.name = xorstr_("particles/entity/env_explosion/explosion_default.vpcf");

	if (g_cfg->world.m_hit_effect_mode == 3) // explosion flashbang
		particle_effect_.name = xorstr_("particles/entity/env_explosion/explosion_flashbang.vpcf");

	if (g_cfg->world.m_hit_effect_mode == 4) // explosion he
		particle_effect_.name = xorstr_("particles/explosions_fx/explosion_hegrenade_brief.vpcf");

	particle_effect_.pad[0] = 8;

	if (g_cfg->world.m_hit_effect_mode == 0) // fire chicken
		g_interfaces->m_game_particle_manager_system->set_effect(bullet.effect_index, 16, &clr, 0);

	g_interfaces->m_game_particle_manager_system->create_effect_index(&bullet.effect_index, &particle_effect_);
	g_interfaces->m_game_particle_manager_system->set_effect(bullet.effect_index, 0, &start, 0);

	if (g_cfg->world.m_hit_effect_mode == 0) // fire chicken
		clr = c_color(100, 0, 0);

	g_interfaces->m_game_particle_manager_system->set_effect(bullet.effect_index, 2, &clr, 0);

	bullet.particle_data_ = {};
}
