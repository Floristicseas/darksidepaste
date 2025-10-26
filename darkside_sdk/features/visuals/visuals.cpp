#include "visuals.hpp"
#include "../../render/render.hpp"
#include "../../entity_system/entity.hpp"
#include "../rage_bot/rage_bot.hpp"
#include "chams.hpp"

namespace visuals
{
	bool Setup()
	{
		if (!chams::chams->initialize())
		{
			printf("failed to initialize DrawObject chams");
			return false;
		}

		return true;
	}

	bool OnDrawObject(void* pAnimatableSceneObjectDesc, void* pDx11, material_data_t* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2)
	{
		return chams::chams->draw_object(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
	}
}

bbox_t c_visuals::calculate_bbox(c_cs_player_pawn* entity)
{
	if (!entity || !entity->is_player_pawn())
		return {};

	bbox_t out{};

	c_game_scene_node* scene_node = entity->m_scene_node();
	if (!scene_node)
		return {};

	c_player_movement_service* movement_services = entity->m_movement_services();
	if (!movement_services)
		return {};

	vec3_t abs_origin = scene_node->m_abs_origin();

	vec3_t bottom = abs_origin - vec3_t{ 0.f, 0.f, 1.f };
	vec3_t top = bottom + vec3_t{ 0.f, 0.f, entity->m_collision()->m_maxs().z };

	vec3_t screen_points[2]{};

	if (g_render->world_to_screen(top, screen_points[1]) && g_render->world_to_screen(bottom, screen_points[0]))
	{
		out.height = std::abs((screen_points[1] - screen_points[0]).y + 6);
		out.width = out.height / 2.0f;

		out.x = screen_points[1].x - out.width / 2.f;
		out.y = screen_points[1].y;

		out.m_found = true;
		return out;
	}

	return {};
}

const char* c_visuals::weapon_to_font_letter(const char* weapon_name) {
	if (!weapon_name)
		return "";

	if (strcmp(weapon_name, "weapon_knife_ct") == 0) return "]";
	if (strcmp(weapon_name, "weapon_knife_t") == 0) return "[";

	if (strcmp(weapon_name, "weapon_deagle") == 0) return "A";
	if (strcmp(weapon_name, "weapon_elite") == 0) return "B";
	if (strcmp(weapon_name, "weapon_fiveseven") == 0) return "C";
	if (strcmp(weapon_name, "weapon_glock") == 0) return "D";
	if (strcmp(weapon_name, "weapon_revolver") == 0) return "J";
	if (strcmp(weapon_name, "weapon_hkp2000") == 0) return "E";
	if (strcmp(weapon_name, "weapon_p250") == 0) return "F";
	if (strcmp(weapon_name, "weapon_usp_silencer") == 0) return "G";
	if (strcmp(weapon_name, "weapon_tec9") == 0) return "H";
	if (strcmp(weapon_name, "weapon_cz75a") == 0) return "I";

	if (strcmp(weapon_name, "weapon_mac10") == 0) return "K";
	if (strcmp(weapon_name, "weapon_ump45") == 0) return "L";
	if (strcmp(weapon_name, "weapon_bizon") == 0) return "M";
	if (strcmp(weapon_name, "weapon_mp7") == 0) return "N";
	if (strcmp(weapon_name, "weapon_mp9") == 0) return "R";
	if (strcmp(weapon_name, "weapon_p90") == 0) return "O";

	if (strcmp(weapon_name, "weapon_galilar") == 0) return "Q";
	if (strcmp(weapon_name, "weapon_famas") == 0) return "R";
	if (strcmp(weapon_name, "weapon_m4a1_silencer") == 0) return "T";
	if (strcmp(weapon_name, "weapon_m4a1") == 0) return "S";
	if (strcmp(weapon_name, "weapon_aug") == 0) return "U";
	if (strcmp(weapon_name, "weapon_sg556") == 0) return "V";
	if (strcmp(weapon_name, "weapon_ak47") == 0) return "W";

	if (strcmp(weapon_name, "weapon_g3sg1") == 0) return "X";
	if (strcmp(weapon_name, "weapon_scar20") == 0) return "Y";
	if (strcmp(weapon_name, "weapon_awp") == 0) return "Z";
	if (strcmp(weapon_name, "weapon_ssg08") == 0) return "a";

	if (strcmp(weapon_name, "weapon_xm1014") == 0) return "b";
	if (strcmp(weapon_name, "weapon_sawedoff") == 0) return "c";
	if (strcmp(weapon_name, "weapon_mag7") == 0) return "d";
	if (strcmp(weapon_name, "weapon_nova") == 0) return "e";

	if (strcmp(weapon_name, "weapon_negev") == 0) return "f";
	if (strcmp(weapon_name, "weapon_m249") == 0) return "g";

	if (strcmp(weapon_name, "weapon_taser") == 0) return "h";

	if (strcmp(weapon_name, "weapon_flashbang") == 0) return "i";
	if (strcmp(weapon_name, "weapon_hegrenade") == 0) return "j";
	if (strcmp(weapon_name, "weapon_smokegrenade") == 0) return "k";
	if (strcmp(weapon_name, "weapon_molotov") == 0) return "l";
	if (strcmp(weapon_name, "weapon_decoy") == 0) return "m";
	if (strcmp(weapon_name, "weapon_incgrenade") == 0) return "n";

	if (strcmp(weapon_name, "weapon_c4") == 0) return "o";

	return "";
}

void c_visuals::store_players() {
	if (!g_interfaces->m_engine->is_in_game())
		return;

	const std::unique_lock<std::mutex> m(m_player_mutex);

	c_cs_player_pawn* local_player = g_ctx->m_local_pawn;

	std::erase_if(m_player_map, [](const auto& kv)
		{
			auto& player_info = kv.second;
			if (player_info.m_valid)
			{
				int current_index = player_info.m_handle == 0xffffffff ? 0x7fff : player_info.m_handle & 0x7fff;

				// in case when handle is still valid
				// entity will be found
				// otherwise it will be fucked OR it will be find wrong entity
				c_base_entity* entity = g_interfaces->m_entity_system->get_base_entity(current_index);

				if (entity)
				{
					c_cs_player_controller* player_controller = reinterpret_cast<c_cs_player_controller*>(entity);

					if (!player_controller)
						return true;

					// if entity is wrong (for example it magically became some unknown prop)
					// erase it
					return !player_controller->is_player();
				}
				else
					return true;
			}

			return false;
		});

	auto entities = g_entity_system->get(xorstr_("CCSPlayerController"));

	for (auto entity : entities)
	{
		c_cs_player_controller* controller = reinterpret_cast<c_cs_player_controller*>(entity);

		if (!controller)
			continue;

		c_cs_player_pawn* player_pawn = reinterpret_cast<c_cs_player_pawn*>(g_interfaces->m_entity_system->get_base_entity(controller->m_pawn().get_entry_index()));
		if (!player_pawn)
			continue;

		int handle = player_pawn->get_handle().to_int();
		int team_num = player_pawn->m_team_num();
		auto player_ptr = m_player_map.find(handle);

		bool is_local_player = (local_player && (player_pawn == local_player));
		bool is_teammate = (!g_cfg->visuals.m_player_esp.m_teammates && (local_player->m_team_num() == team_num));

		if (is_local_player || is_teammate || !player_pawn->is_alive()) {

			if (player_ptr != m_player_map.end())
				m_player_map.erase(player_ptr);

			continue;
		}

		if (player_ptr == m_player_map.end()) {
			m_player_map.insert_or_assign(handle, player_info_t{});
			continue;
		}

		player_info_t& player_info = player_ptr->second;
		player_info.m_valid = true;
		player_info.m_handle = handle;
		player_info.m_bbox = calculate_bbox(player_pawn);
		player_info.m_health = player_pawn->m_health();
		player_info.m_name = controller->m_player_name();

		c_base_player_weapon* active_weapon = player_pawn->get_active_weapon();
		if (active_weapon) {
			c_cs_weapon_base_v_data* weapon_data = active_weapon->get_weapon_data();
			if (weapon_data) {
				player_info.m_ammo = active_weapon->m_clip1();
				player_info.m_max_ammo = weapon_data->m_max_clip1();

				const char* m_name = weapon_data->m_name();
				const char* name_length = strstr(m_name, "weapon_");
				std::string weapon_name = name_length ? name_length + strlen("weapon_") : m_name;
				std::transform(weapon_name.begin(), weapon_name.end(), weapon_name.begin(), ::toupper);

				player_info.m_weapon_name = std::move(weapon_name);

				player_info.m_weapon_icon = weapon_to_font_letter(m_name);
			}
		}
	}
}

void c_visuals::handle_players() {
	const std::unique_lock<std::mutex> m(m_player_mutex);

	for (auto it = m_player_map.begin(); it != m_player_map.end(); it = std::next(it)) {
		player_info_t& player_info = it->second;
		if (!player_info.m_valid)
			continue;

		if (player_info.m_bbox.m_found) {
			bbox_t bbox = player_info.m_bbox;

			static const c_color outline_color = c_color{ 0.f, 0.f, 0.f, 1.f };

			if (g_cfg->visuals.m_player_esp.m_bounding_box) {
				const c_color main_color = c_color{ 1.f, 1.f, 1.f, 1.f };

				g_render->rect_outline({ bbox.x - 1, bbox.y - 1 }, { bbox.x + bbox.width + 1, bbox.y + bbox.height + 1 }, outline_color);
				g_render->rect_outline({ bbox.x, bbox.y }, { bbox.x + bbox.width, bbox.y + bbox.height }, main_color);
				g_render->rect_outline({ bbox.x + 1, bbox.y + 1 }, { bbox.x + bbox.width - 1, bbox.y + bbox.height - 1 }, outline_color);
			}
			if (g_cfg->visuals.m_player_esp.m_health_bar) {
				c_color full_hp_color = c_color{ 0.69f, 1.f, 0.31f, 1.f };
				c_color low_hp_color = c_color{ 1.f, 0.2f, 0.31f, 1.f };

				int esp_health = std::clamp(player_info.m_health, 0, 100);
				float esp_step = std::clamp(static_cast<float>(esp_health) / 100.f, 0.f, 1.f);

				float height = (bbox.height - ((bbox.height * static_cast<float>(esp_health)) / 100.f));
				height = std::max<float>(std::min<float>(height, bbox.height), 0.f);

				vec3_t bb_outline_min{ bbox.x - 5.f, bbox.y - 1.f };
				vec3_t bb_outline_max{ 3.f, bbox.height + 2.f };

				vec3_t bb_min{ bbox.x - 4.f, bbox.y + height };
				vec3_t bb_max{ 2.f, bbox.height - height };

				g_render->rect(bb_outline_min, bb_outline_min + bb_outline_max, outline_color);
				g_render->rect(bb_min, bb_min + bb_max, low_hp_color.lerp(full_hp_color, esp_step));
			}

			if (g_cfg->visuals.m_player_esp.m_name) {
				const c_color main_color = (g_cfg->visuals.m_player_esp.m_name_color * 255.f);

				ImGui::PushFont(g_render->fonts.verdana_small);
				auto text_size = ImGui::CalcTextSize(player_info.m_name.c_str());

				g_render->text(vec3_t(bbox.x + bbox.width / 2.f, bbox.y - text_size.y - 3.f),
					main_color, font_flags_center | font_flags_dropshadow,
					g_render->fonts.verdana_small, player_info.m_name,
					g_render->fonts.verdana_small->FontSize);

				ImGui::PopFont();
			}

			float weapon_info_offset = 0.f;

			if (g_cfg->visuals.m_player_esp.m_weapon) {
				const auto main_color = c_color{ 1.f, 1.f, 1.f, 1.f };

				auto full_ammo_color = c_color{ 0.67f, 0.725f, 0.98f, 1.f };
				auto low_ammo_color = c_color{ 1.f, 33.f, 33.f, 1.f };

				if (player_info.m_ammo > 0) {
					float esp_step = std::clamp(static_cast<float>(player_info.m_ammo) / static_cast<float>(player_info.m_max_ammo), 0.f, 1.f);

					float width = ((bbox.width * static_cast<float>(player_info.m_ammo)) / static_cast<float>(player_info.m_max_ammo));
					width = std::max<float>(std::min<float>(width, bbox.width), 0.f);

					vec3_t bb_outline_min{ bbox.x - 1.f, bbox.y + bbox.height + 2.f };
					vec3_t bb_outline_max{ bbox.width + 2.f, 2.f };

					vec3_t bb_min{ bbox.x, bbox.y + bbox.height + 2.f };
					vec3_t bb_max{ width, 2.f };

					g_render->rect(bb_outline_min, bb_outline_min + bb_outline_max, outline_color);
					g_render->rect(bb_min, bb_min + bb_max, low_ammo_color.lerp(full_ammo_color, esp_step));

					weapon_info_offset += 5.f;
				}

				g_render->text(vec3_t(bbox.x + bbox.width / 2.f, bbox.y + bbox.height + weapon_info_offset + 3.f), main_color, font_flags_center | font_flags_outline,
					g_render->fonts.onetap_pixel, player_info.m_weapon_name, g_render->fonts.onetap_pixel->FontSize);

				weapon_info_offset += 12.f; 
			}

			// Weapon icon rendering
			if (g_cfg->visuals.m_player_esp.m_weapon_icons && !player_info.m_weapon_icon.empty()) {
				const auto icon_color = c_color{ 1.f, 1.f, 1.f, 1.f };

				ImGui::PushFont(g_render->fonts.GunIcons); 
				auto icon_size = ImGui::CalcTextSize(player_info.m_weapon_icon.c_str());

				g_render->text(vec3_t(bbox.x + bbox.width / 2.f, bbox.y + bbox.height + weapon_info_offset + 3.f),
					icon_color, font_flags_center | font_flags_dropshadow,
					g_render->fonts.GunIcons, player_info.m_weapon_icon,
					12/*g_cfg->visuals.debug_icon */ /*g_render->fonts.GunIcons->FontSize*/);

				ImGui::PopFont();
			}
		}
	}
}

void c_visuals::on_present() {
	handle_players();
}