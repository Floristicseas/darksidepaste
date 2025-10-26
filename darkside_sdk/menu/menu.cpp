#include "menu.hpp"

#include "../darkside.hpp"

#include "../features/skins/skins.hpp"

void c_menu::draw() {
    if (!m_opened)
        return;

    ImGui::SetNextWindowSize(ImVec2(512.0f, 515.0f), ImGuiCond_Once);
    ImGui::Begin("cs2", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::BeginChild("tabs", ImVec2{ 150, 0 }, true);
    {
        static constexpr const char* tabs[]{ "Rage", "Anti-aim", "World esp", "Player esp", "Chams", "Misc", "Skins", "Settings" };

        for (std::size_t i{ }; i < IM_ARRAYSIZE(tabs); ++i) {
            if (ImGui::Selectable(tabs[i], m_selected_tab == i)) {
                m_selected_tab = i;
            }
        }
    }
    ImGui::EndChild();
    ImGui::SameLine();

    if (m_selected_tab == 0) {
        ImGui::BeginChild("ragebot", ImVec2{ 0, 0 }, true);
        {
            const char* hitboxes[] = { "head", "chest", "stomach", "arms", "legs", "feet" };
            const char* multi_points[] = { "head", "chest", "stomach", "arms", "legs", "feet" };

            ImGui::Checkbox("enabled", &g_cfg->rage_bot.m_enabled);
            ImGui::Checkbox("silent", &g_cfg->rage_bot.m_silent);
            ImGui::MultiCombo("hiboxes", g_cfg->rage_bot.m_hitboxes, hitboxes, IM_ARRAYSIZE(hitboxes));
            ImGui::MultiCombo("multi points", g_cfg->rage_bot.m_multi_points, multi_points, IM_ARRAYSIZE(multi_points));

            if (g_cfg->rage_bot.m_multi_points[0]
                || g_cfg->rage_bot.m_multi_points[1]
                || g_cfg->rage_bot.m_multi_points[2]
                || g_cfg->rage_bot.m_multi_points[3]
                || g_cfg->rage_bot.m_multi_points[4]
                || g_cfg->rage_bot.m_multi_points[5]) {
                ImGui::Checkbox("static scale", &g_cfg->rage_bot.m_static_scale);

                if (g_cfg->rage_bot.m_static_scale) {
                    for (int i = 0; i < IM_ARRAYSIZE(multi_points); i++) {
                        if (g_cfg->rage_bot.m_multi_points[i])
                            ImGui::SliderInt(std::format("{0} scale", multi_points[i]).c_str(), &g_cfg->rage_bot.m_hitbox_scale[i], 1, 100);
                    }
                }
            }

            ImGui::SliderInt("minimum damage", &g_cfg->rage_bot.m_minimum_damage, 1, 100);
            ImGui::SliderInt("damage override", &g_cfg->rage_bot.m_minimum_damage_override, 1, 100);
            ImGui::Keybind("damage override keybind", &g_cfg->rage_bot.m_override_damage_key_bind, &g_cfg->rage_bot.m_override_damage_key_bind_style);
            ImGui::SliderInt("hit chance", &g_cfg->rage_bot.m_hit_chance, 1, 100);
            ImGui::Checkbox("auto fire", &g_cfg->rage_bot.m_auto_fire);
            ImGui::Checkbox("auto stop", &g_cfg->rage_bot.m_auto_stop);
        }
        ImGui::EndChild();
    }

    if (m_selected_tab == 1) {
        ImGui::BeginChild("anti-hit", ImVec2{ 0, 0 }, true);
        {
            ImGui::Checkbox("enabled", &g_cfg->anti_hit.m_enabled);
            ImGui::Checkbox("override pitch", &g_cfg->anti_hit.m_override_pitch);
            if (g_cfg->anti_hit.m_override_pitch)
                ImGui::SliderInt("##override_pitch", &g_cfg->anti_hit.m_pitch_amount, -89, 89);
            ImGui::Checkbox("override yaw", &g_cfg->anti_hit.m_override_yaw);
            if (g_cfg->anti_hit.m_override_yaw) {
                ImGui::Checkbox("at target", &g_cfg->anti_hit.m_at_target);
                ImGui::SliderInt("##override_yaw", &g_cfg->anti_hit.m_yaw_amount, 0, 180);
                ImGui::SliderInt("jiter amount", &g_cfg->anti_hit.m_jitter_amount, 0, 180);
            }
        }
        ImGui::EndChild();
    }

    if (m_selected_tab == 2) {
        ImGui::BeginChild("world esp ", ImVec2{ 0, 0 }, true);
        {
            ImGui::Checkbox("enabled thirdperson", &g_cfg->world_esp.m_enable_thirdperson);
            ImGui::SliderInt("override distance", &g_cfg->world_esp.m_distance, 0, 180);

            ImGui::Keybind("thirdperson keybind", &g_cfg->world_esp.m_thirdperson_key_bind, &g_cfg->world_esp.m_thirdperson_key_bind_style);

            ImGui::Checkbox("render fog", &g_cfg->world.m_render_fog);
            ImGui::ColorEdit4("sky color", reinterpret_cast<float*>(&g_cfg->world.m_sky), ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("sky and clouds color", reinterpret_cast<float*>(&g_cfg->world.m_sky_clouds), ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("wall color", reinterpret_cast<float*>(&g_cfg->world.m_wall), ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("lighting color", reinterpret_cast<float*>(&g_cfg->world.m_lighting), ImGuiColorEditFlags_NoInputs);
            ImGui::SliderInt("exposure ", reinterpret_cast<int*>(&g_cfg->world.m_exposure), 1, 100);

            ImGui::SliderFloat("Aspect Ratio ", reinterpret_cast<float*>(&g_cfg->misc.m_aspect_ratio), 0, 4);

            ImGui::Separator();

            ImGui::Checkbox("Change viewmodel", &g_cfg->world_esp.viewchanger);

            if (g_cfg->world_esp.viewchanger) {
                ImGui::SliderInt("X ", &g_cfg->world_esp.flX, -90, 90);
                ImGui::SliderInt("Y ", &g_cfg->world_esp.flY, -90, 90);
                ImGui::SliderInt("Z ", &g_cfg->world_esp.flZ, -90, 90);
                ImGui::SliderInt("FOV ", &g_cfg->world_esp.flfov, 40, 120);

                // ImGui::Separator();
                // ImGui::Text("Rotation");

                // ImGui::SliderInt("X ##r", &g_cfg->world_esp.flXrotation, -90, 90);
                // ImGui::SliderInt("Y ##r", &g_cfg->world_esp.flYrotation, -90, 90);
                // ImGui::SliderInt("Z ##r", &g_cfg->world_esp.flZrotation, -90, 90);           
            }
        }
        ImGui::EndChild();
    }

    if (m_selected_tab == 3) {
        ImGui::BeginChild("player esp ", ImVec2{ 0, 0 }, true);
        {
            ImGui::Checkbox("teammates", &g_cfg->visuals.m_player_esp.m_teammates);
            ImGui::Checkbox("bounding box", &g_cfg->visuals.m_player_esp.m_bounding_box);
            ImGui::Checkbox("health bar", &g_cfg->visuals.m_player_esp.m_health_bar);
            ImGui::Checkbox("name", &g_cfg->visuals.m_player_esp.m_name);
            ImGui::Checkbox("weapon", &g_cfg->visuals.m_player_esp.m_weapon);
            ImGui::Checkbox("weapon icons", &g_cfg->visuals.m_player_esp.m_weapon_icons);
            //ImGui::SliderInt(xorstr_("D3bug icon"), &g_cfg->visuals.debug_icon, 0, 179);

            ImGui::ColorEdit4("name color", reinterpret_cast<float*>(&g_cfg->visuals.m_player_esp.m_name_color), ImGuiColorEditFlags_NoInputs);

            ImGui::Separator();

            ImGui::Checkbox("Local Glow", &g_cfg->visuals.glow.m_bGlow_local);

            ImGui::ColorEdit4("local glow color", reinterpret_cast<float*>(&g_cfg->visuals.glow.glow_color_local), ImGuiColorEditFlags_NoInputs);

        }
        ImGui::EndChild();
    }

    if (m_selected_tab == 4) {
        ImGui::BeginChild("chams", ImVec2{ 0, 0 }, true);
        {
           /*static constexpr const char* material_types[]{"Solid", "Flat", "Glow", "Latex", "Star", "Electric"};

            // Enemy Chams
            ImGui::Text("Enemy Chams");
            ImGui::Checkbox("Visible", &g_cfg->visuals.chams.m_enemy.m_visible);
            ImGui::Checkbox("Occluded", &g_cfg->visuals.chams.m_enemy.m_occluded);
            ImGui::Combo("Enemy Material", &g_cfg->visuals.chams.m_enemy.m_type, material_types, IM_ARRAYSIZE(material_types));

            if (g_cfg->visuals.chams.m_enemy.m_visible)
                ImGui::ColorEdit4("Enemy Visible Color", reinterpret_cast<float*>(&g_cfg->visuals.chams.m_enemy.m_visible_color), ImGuiColorEditFlags_NoInputs);

            if (g_cfg->visuals.chams.m_enemy.m_occluded)
                ImGui::ColorEdit4("Enemy Occluded Color", reinterpret_cast<float*>(&g_cfg->visuals.chams.m_enemy.m_occluded_color), ImGuiColorEditFlags_NoInputs);

            ImGui::Separator();

            // Local Chams
            ImGui::Text("Local Chams");
            ImGui::Checkbox("Local Enabled", &g_cfg->visuals.chams.m_local.m_enabled);
            ImGui::Checkbox("Local Occluded", &g_cfg->visuals.chams.m_local.m_occluded);
            ImGui::Combo("Local Material", &g_cfg->visuals.chams.m_local.m_type, material_types, IM_ARRAYSIZE(material_types));

            ImGui::ColorEdit4("Local Color", reinterpret_cast<float*>(&g_cfg->visuals.chams.m_local.m_color), ImGuiColorEditFlags_NoInputs);

            if (g_cfg->visuals.chams.m_local.m_occluded)
                ImGui::ColorEdit4("Local Occluded Color", reinterpret_cast<float*>(&g_cfg->visuals.chams.m_local.m_occluded_color), ImGuiColorEditFlags_NoInputs);

            ImGui::Separator();

            // Weapon Chams
            ImGui::Text("Weapon Chams");
            ImGui::Checkbox("Weapon Enabled", &g_cfg->visuals.chams.m_weapon.m_enabled);
            ImGui::Combo("Weapon Material", &g_cfg->visuals.chams.m_weapon.m_type, material_types, IM_ARRAYSIZE(material_types));
            ImGui::ColorEdit4("Weapon Color", reinterpret_cast<float*>(&g_cfg->visuals.chams.m_weapon.m_color), ImGuiColorEditFlags_NoInputs);*/
        } 
        ImGui::EndChild();
    }

    if (m_selected_tab == 5) {
        ImGui::BeginChild("misc", ImVec2{ 165, 0 }, true);
        {
            const char* removal_items[] = { "Remove scope", "Remove legs", "Remove crosshair", "Remove flash", "Remove smoke", "Remove team intro", "Remove local overhead", "Remove aim punch" };

            ImGui::SliderInt(xorstr_("Field of View"), &g_cfg->misc.m_field_of_view, 1, 179);
            ImGui::MultiCombo("Removals", g_cfg->misc.m_removals, removal_items, IM_ARRAYSIZE(removal_items));

            if (g_cfg->misc.m_removals[0]) {
                ImGui::Checkbox("Custom scope", &g_cfg->world_esp.custom_crosshair);
            }

            if (g_cfg->world_esp.custom_crosshair && g_cfg->misc.m_removals[0]) {
                ImGui::SliderInt("Lines offset ", reinterpret_cast<int*>(&g_cfg->world_esp.scope_lines_offset), 1, 1000);
                ImGui::SliderInt("Fade speed ", reinterpret_cast<int*>(&g_cfg->world_esp.scope_lines_fade_speed), 1, 1000);
                ImGui::SliderInt("Intial Pos ", reinterpret_cast<int*>(&g_cfg->world_esp.scope_lines_initial_pos), 1, 1000);
                ImGui::ColorEdit4("Scope color ", reinterpret_cast<float*>(&g_cfg->world_esp.scope_lines_color), ImGuiColorEditFlags_NoInputs);
            }

        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("misc_movement", ImVec2{ 165, 0 }, true);
        {
            static constexpr const char* strafe_types[]{ "Directional", "View Angle" };
            static constexpr const char* directional_types[]{ "Default", "Rage" };

            ImGui::Checkbox(xorstr_("Quick Stop"), &g_cfg->misc.m_quick_stop);
            ImGui::Checkbox("Bhop", &g_cfg->misc.m_hop);
            ImGui::Checkbox("auto strafe", &g_cfg->misc.m_auto_strafe);

            if (g_cfg->misc.m_auto_strafe) {
                ImGui::Text("Smoothness");
                ImGui::SliderInt("##auto_strafer_smooth", &g_cfg->misc.m_strafe_smooth, 1, 99);

                ImGui::Combo("auto strafe type", &g_cfg->misc.m_auto_strafe_type, strafe_types, IM_ARRAYSIZE(strafe_types));

                if (g_cfg->misc.m_auto_strafe_type == 0) { // Directional
                   // ImGui::Combo("Directional type", &g_cfg->misc.m_directional_type, directional_types, IM_ARRAYSIZE(directional_types));
                }
                ImGui::Checkbox("Silent strafe", &g_cfg->misc.m_silent_strafe);

            }


            ImGui::Checkbox("auto peek", &g_cfg->misc.m_auto_peek);

            if (g_cfg->misc.m_auto_peek) {
                ImGui::Keybind(xorstr_("Auto Peek Keybind"), &g_cfg->misc.m_override_quick_peek_assistant, &g_cfg->misc.m_override_quick_peek_assistant_style);
                ImGui::Checkbox("return on key release", &g_cfg->misc.m_auto_peek_return);
            }



        }
        ImGui::EndChild();
    }

    if (m_selected_tab == 6) {
        ImGui::BeginChild("misc", ImVec2{ 0, 0 }, true);
        {
          /* static constexpr const char* type[]{"weapons", "knives", "gloves", "agents"};
            ImGui::Combo("Types", &g_cfg->skins.m_selected_type, type, IM_ARRAYSIZE(type));

            if (g_cfg->skins.m_selected_type == 0) {
                static std::vector<const char*> weapon_names{};

                if (weapon_names.size() < g_skins->m_dumped_items.size())
                    for (auto& item : g_skins->m_dumped_items)
                        weapon_names.emplace_back(item.m_name);

                ImGui::Combo("Weapons", &g_cfg->skins.m_selected_weapon, weapon_names.data(), weapon_names.size());
                auto& selected_entry = g_cfg->skins.m_skin_settings[g_cfg->skins.m_selected_weapon];

                if (ImGui::BeginListBox("##skins"))
                {
                    auto& selected_weapon_entry = g_skins->m_dumped_items[g_cfg->skins.m_selected_weapon];

                    for (auto& skin : selected_weapon_entry.m_dumped_skins)
                    {
                        ImGui::PushID(&skin);
                        if (ImGui::Selectable(skin.m_name, selected_entry.m_paint_kit == skin.m_id))
                        {
                            if (selected_weapon_entry.m_selected_skin == &skin)
                                selected_weapon_entry.m_selected_skin = nullptr;
                            else
                            {
                                selected_weapon_entry.m_selected_skin = &skin;
                                selected_entry.m_paint_kit = skin.m_id;
                            }
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndListBox();
                }
            }

            if (g_cfg->skins.m_selected_type == 1)
                ImGui::Combo("##knifes", &g_cfg->skins.m_knives.m_selected, g_skins->m_knives.m_dumped_knife_name.data(), g_skins->m_knives.m_dumped_knife_name.size());

            if (g_cfg->skins.m_selected_type == 2)
                ImGui::Combo("##gloves", &g_cfg->skins.m_gloves.m_selected, g_skins->m_gloves.m_dumped_glove_name.data(), g_skins->m_gloves.m_dumped_glove_name.size());

            if (g_cfg->skins.m_selected_type == 3) {
                ImGui::Combo("##agents ct", &g_cfg->skins.m_agents.m_selected_ct, g_skins->m_agents.m_dumped_agent_name.data(), g_skins->m_agents.m_dumped_agent_name.size());
                ImGui::Combo("##agents t", &g_cfg->skins.m_agents.m_selected, g_skins->m_agents.m_dumped_agent_name.data(), g_skins->m_agents.m_dumped_agent_name.size());
            }*/ 
        }
        ImGui::EndChild();
    }

    if (m_selected_tab == 7) {
        g_config_system->menu();
    }

    ImGui::End();
}

void c_menu::on_create_move()
{
    if (!m_opened)
        return;

    g_ctx->m_user_cmd->m_button_state.m_button_state &= ~(IN_ATTACK | IN_ATTACK2);
    g_ctx->m_user_cmd->m_button_state.m_button_state2 &= ~(IN_ATTACK | IN_ATTACK2);
    g_ctx->m_user_cmd->m_button_state.m_button_state3 &= ~(IN_ATTACK | IN_ATTACK2);
}