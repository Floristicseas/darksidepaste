#pragma once

#include "../../darkside.hpp"

class material_data_t;

namespace visuals
{
	bool Setup();
	bool OnDrawObject(void* pAnimatableSceneObjectDesc, void* pDx11, material_data_t* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2);
}

struct bbox_t {
	bool m_found = false;

	float x;
	float y;
	float width;
	float height;
};

class c_visuals {
	struct player_info_t {
		bool m_valid = false;

		int m_handle;
		int m_health;
		int m_ammo;
		int m_max_ammo;

		bbox_t m_bbox;

		std::string m_name;
		std::string m_weapon_name;
		std::string m_weapon_icon;
	};

	bbox_t calculate_bbox(c_cs_player_pawn* entity);
	const char* weapon_to_font_letter(const char* weapon_name); 
	void draw_skeleton(c_cs_player_pawn* player_pawn, const c_color& color);
	void draw_off_arrow(c_cs_player_pawn* local_player, c_cs_player_pawn* enemy_pawn);

	std::mutex m_player_mutex;
	std::unordered_map<int, player_info_t> m_player_map;

	void handle_players();
public:
	void store_players();
	void on_present();
};


inline const auto g_visuals = std::make_unique<c_visuals>();