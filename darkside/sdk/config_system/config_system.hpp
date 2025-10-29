#pragma once
#include "../typedefs/c_color.hpp"

#include <vector>
#include <string>

#include <memory>

class c_dumped_item;

#define g_cfg g_config_system

template <typename T>
struct item_t {
	T* pointer;
	std::string category;
	std::string name;
	T default_value;
};

class c_config_system {
private:
	std::vector<item_t<bool>> booleans;
	std::vector<item_t<int>> ints;
	std::vector<item_t<float>> floats;
	std::vector<item_t<int>> keybinds;
	std::vector<item_t<std::string>> strings;
	std::vector<item_t<c_color>> colors_rgb;
public:
	void push_item( bool* pointer, std::string category, std::string name, bool def_value );
	void push_item( int* pointer, std::string category, std::string name, int def_value );
	void push_item( float* pointer, std::string category, std::string name, float def_value );
	void push_item( c_color* pointer, std::string category, std::string name, c_color def_value );

	void setup_values( );
	void read( std::string path );
	void save( std::string path );
	void remove( std::string path );
	void refresh( );
	void reset( );
	void menu( );
	std::vector<std::string> config_files;

	struct rage_bot_t
	{
		bool m_enabled;
		bool m_hitboxes[ 6 ];
		bool m_multi_points[ 6 ];
		bool m_static_scale;
		bool m_silent = true;
		int m_hitbox_scale[ 6 ] = { 50 };
		int m_minimum_damage = 50;
		int m_minimum_damage_override = 10;
		int m_hit_chance = 50;
		bool m_auto_fire = true;
		bool m_auto_stop;

		int m_override_damage_key_bind, m_override_damage_key_bind_style;

		// TO-DO: make settings for weapon groups
		struct {

		} weapon_group[ 7 ];
	} rage_bot;

	struct anti_hit_t
	{
		bool m_enabled;
		bool m_at_target;
		bool m_override_pitch;
		bool m_override_yaw;
		int m_pitch_amount = 0;
		int m_yaw_amount = 0;
		int m_jitter_amount = 0;
	} anti_hit;

	struct visuals_t
	{
		struct player_esp_t {
			bool m_bounding_box = true;
			c_color m_bounding_box_col = c_color(0.36f, 0.36f, 0.36f, 1.f);
			bool m_health_bar = true;
			bool m_name = true;
			bool m_weapon = true;
			c_color m_weapon_color = c_color(0.36f, 0.36f, 0.36f, 1.f);
			bool m_flags = true;
			bool m_teammates = false;
			bool m_weapon_icons = false;
			c_color m_weapon_icon_col = c_color(0.36f, 0.36f, 0.36f, 1.f);

			bool m_skeleton = true;
			c_color m_skeleton_color = c_color(1.f, 1.f, 1.f, 1.f);
			bool m_off_arrow = true;
			c_color m_off_arrow_color = c_color(1.f, 0.2f, 0.2f, 1.f);

			c_color m_name_color{ 1.f, 1.f, 1.f, 1.f };
		}m_player_esp;

		int debug_icon = 0;	

		struct glow_t {
			bool m_bGlow;
			bool m_bGlow_local;
			bool m_bGlowOnRagdoll;
			c_color glow_color = c_color(255, 255, 255, 255);
			c_color glow_color_local = c_color(0.36f, 0.36f, 0.36f);
		} glow; 

		struct chams_t {
			struct enemy_t {
				bool m_visible = true;
				bool m_occluded = false;
				int m_type = 0;
				c_color m_visible_color = c_color(1.f, 1.f, 1.f, 1.f);
				c_color m_occluded_color = c_color(1.f, 1.f, 1.f, 1.f);
			} m_enemy;

			struct local_t {
				bool m_enabled = true;
				bool m_occluded = false;
				bool m_visible = true;
				int m_type = 0;
				c_color m_color = c_color(1.f, 1.f, 1.f, 1.f);
				c_color m_occluded_color = c_color(1.f, 1.f, 1.f, 1.f);
			} m_local;

			struct weapon_t {
				bool m_enabled = false;
				int m_type = 0;
				c_color m_color = c_color(1.f, 1.f, 1.f, 1.f);
			} m_weapon;
		} chams;

	} visuals;

	struct world_esp_t {
		bool m_enable_thirdperson;
		bool m_enable_override_fov;
		int m_distance;
		bool viewchanger;
		int m_override_fov;
		bool custom_crosshair;
		bool enable_aspect;
		int scope_lines_initial_pos = 190;
		int scope_lines_offset = 0;
		int scope_lines_fade_speed = 12;
//		c_color scope_lines_color = c_color(1.f, 1.f, 1.f, 1.f);
		c_color scope_lines_color = c_color(255, 255, 255, 255);

		int flX;
		int flY;
		int flZ;
		int flXrotation;
		int flYrotation;
		int flZrotation;
		int flfov;

		int m_thirdperson_key_bind, m_thirdperson_key_bind_style;
		int m_aspect_ratio = 10;
	} world_esp;

	struct world_modulation_t {
		c_color m_sky{ 0.36f, 0.36f, 0.36f }, m_sky_clouds{ 0.37f, 0.84f, 0.98f, 1.f }, m_wall{ 1.f, 1.f, 1.f, 1.f }, m_lighting{ 0.35f, 0.35f, 0.52f };
		int m_exposure = 20;

		int m_hit_effect_mode;
		bool m_bullet_tracer;
		int m_bullet_tracer_mode;
		bool m_hit_effect;

		bool m_render_fog;	
		bool m_gred_pred;
	} world;

	struct misc_t
	{
		bool m_auto_peek;
		bool m_auto_peek_return;
		bool m_quick_stop;
		bool m_silent_strafe;
		int m_auto_strafe_type;
		int m_directional_type;		
		bool m_hop;
		bool m_auto_strafe = false;
		int m_field_of_view = 90;
		int m_strafe_smooth = 70;
		int m_quick_peek_assistant_type = 0;
		int m_override_quick_peek_assistant, m_override_quick_peek_assistant_style;
		c_color hitmark_clr{ 1.f, 1.f, 1.f };
		float m_aspect_ratio_debug;
		int m_hit_sound_mode;
		int m_hitmark_mode;
		bool m_hitsound;
		bool m_hitmark;
		bool m_hit_logs;
		
		bool damageonenemy;
		c_color damageclr{ 1.f, 1.f, 1.f };
		
		bool m_removals[7];
	} misc;

	struct indicators_t {
		int keybindposx;
		int keybindposy;

		int watermarkposx;
		int watermarkposy;

		bool m_indicators[1];

	} indicators;

	struct skins_t {
		struct
		{
			int m_selected = 0;
			int m_selected_ct = 0;
			bool m_agent_changer;
		} m_agents;

		struct
		{
			bool m_knife_changer;
			int m_selected = 0;
		} m_knives;

		bool m_custom_models;

		int m_selected_weapon = 0;
		int m_selected_type = 0;
		c_dumped_item* m_selected_item = nullptr;
	} skins;
};

inline const auto g_config_system = std::make_unique<c_config_system>( );