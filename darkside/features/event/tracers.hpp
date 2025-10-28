#pragma once

#include "../../darkside.hpp"
#include "../../sdk/typedefs/c_strong_handle.hpp"
#include <mutex>

class particle_effect
{
public:
	const char* name{};
	char pad[0x30]{};
};

class particle_information
{
public:
	float time{};
	float width{};
	float unk2{};
};

class particle_data
{
public:
	vec3_t* positions{};
	char pad[0x74]{};
	float* times{};
	void* unk_ptr{};
	char pad2[0x28];
	float* times2{};
	char pad3[0x98];
	void* unk_ptr2{};
};

struct particle_color
{
	float r;
	float g;
	float b;
};

class c_particle_snapshot {
public:
	void draw(int count, void* data);
};

class c_game_particle_manager_system
{
public:
	void create_effect_index(unsigned int* effect_index, particle_effect* effect_data);
	void set_effect(unsigned int effect_index, int unk, void* clr, int unk2);
	void fn_init_effect(int effect_index, unsigned int unk, const c_strong_handle< c_particle_snapshot >* particle_snapshot);
};

class c_particle_manager
{
public:
	void create_snapshot(c_strong_handle< c_particle_snapshot >* out_particle_snapshot);
	void draw(c_strong_handle< c_particle_snapshot >* particle_snapshot, int count, void* data);
};

class tracer_info
{
public:
	unsigned int effect_index = -1;

	vec3_t* positions = nullptr;

	float* times = nullptr;

	c_strong_handle< c_particle_snapshot > handle_snapshot_particle{};
	particle_data particle_data_;
};

class cbullet_tracer
{
public:
	void add_bullet_trace(vec3_t start, vec3_t end, c_color clr_);
	void add_hit_effect(vec3_t start, c_color clr);
private:
	std::mutex MHitMutex;

	class cbullet_tracer_info
	{
	public:
		cbullet_tracer_info(vec3_t src, vec3_t dst, float time, c_color color)
		{
			this->src = src;
			this->dst = dst;
			this->time = time;
			this->color = color;
		}

		vec3_t src, dst;
		float time;
		c_color color;
	};
};

inline std::unique_ptr< cbullet_tracer > g_bullet_tracer = std::make_unique< cbullet_tracer >();