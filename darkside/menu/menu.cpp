#include "menu.hpp"
#include "../darkside.hpp"
#include "../features/skins/skins.hpp"
#include "../features/event/logs/logs.hpp"
#include "custom/keybinds.hpp"
#include "custom/watermark.hpp"

void c_menu::style_setup() {
    auto& style = ImGui::GetStyle();
    style.WindowRounding = 12.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.PopupRounding = 8.0f;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 0.1f);
}

void c_menu::draw_bg() {
    if (!m_opened)
        return;

    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();

    ImGui::GetWindowDrawList()->AddRectFilled(
        window_pos,
        ImVec2(window_pos.x + window_size.x, window_pos.y + window_size.y),
        IM_COL32(10, 10, 15, 180)
    );

    ImGui::GetWindowDrawList()->AddRect(
        window_pos,
        ImVec2(window_pos.x + window_size.x, window_pos.y + window_size.y),
        IM_COL32(255, 255, 255, 30),
        12.0f, 0, 2.0f
    );

}

void c_menu::draw() {

    g_event_logs->render();

    g_keybinds_display->render();

    g_watermark_display->render();

    if (!m_opened)
        return;

    style_setup();

    ImGui::SetNextWindowSize(ImVec2(512.0f, 515.0f), ImGuiCond_Once);

    ImGui::Begin("cs2", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

    draw_bg();

    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();

    draw->AddRectFilledMultiColor(
        pos,
        ImVec2(pos.x + size.x, pos.y + 26.f),
        IM_COL32(0, 180, 255, 255),
        IM_COL32(0, 180, 255, 255),
        IM_COL32(0, 120, 255, 255),
        IM_COL32(0, 120, 255, 255)
    );

    ImGui::SetCursorPosY(4.f);
    ImGui::SetCursorPosX(10.f);
    ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "darkside sdk");

    ImGui::SetCursorPosY(30.f);

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
            static const char* effects[] = { ("fire chicken"), ("sparks"), ("basic explosion"), ("flashbang explosion"), ("he explosion") };
            static const char* tracers[] = { ("Line"), ("Beam") };

            ImGui::Checkbox("enabled thirdperson", &g_cfg->world_esp.m_enable_thirdperson);
            ImGui::SliderInt("override distance", &g_cfg->world_esp.m_distance, 0, 180);

            ImGui::Keybind("thirdperson keybind", &g_cfg->world_esp.m_thirdperson_key_bind, &g_cfg->world_esp.m_thirdperson_key_bind_style);

            ImGui::Checkbox(xorstr_("Grenade Prediction"), &g_cfg->world.m_gred_pred);

            ImGui::Checkbox("render fog", &g_cfg->world.m_render_fog);
            ImGui::ColorEdit4("sky color", reinterpret_cast<float*>(&g_cfg->world.m_sky), ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("sky and clouds color", reinterpret_cast<float*>(&g_cfg->world.m_sky_clouds), ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("wall color", reinterpret_cast<float*>(&g_cfg->world.m_wall), ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("lighting color", reinterpret_cast<float*>(&g_cfg->world.m_lighting), ImGuiColorEditFlags_NoInputs);
            ImGui::SliderInt("exposure ", reinterpret_cast<int*>(&g_cfg->world.m_exposure), 1, 100);

            ImGui::Checkbox(xorstr_("Bullet tracers"), &g_cfg->world.m_bullet_tracer);

            if (g_cfg->world.m_bullet_tracer)
                ImGui::Combo(xorstr_("##bullet_tracer_mode"), &g_cfg->world.m_bullet_tracer_mode, tracers, IM_ARRAYSIZE(tracers), sizeof(tracers));

            ImGui::Checkbox(xorstr_("Hit effects"), &g_cfg->world.m_hit_effect);

            if (g_cfg->world.m_hit_effect)
                ImGui::Combo(xorstr_("##hit_effect_mode"), &g_cfg->world.m_hit_effect_mode, effects, IM_ARRAYSIZE(effects), sizeof(effects));

            ImGui::Separator();

            ImGui::Checkbox("Change viewmodel", &g_cfg->world_esp.viewchanger);

            if (g_cfg->world_esp.viewchanger) {
                ImGui::SliderInt("X ", &g_cfg->world_esp.flX, -90, 90);
                ImGui::SliderInt("Y ", &g_cfg->world_esp.flY, -90, 90);
                ImGui::SliderInt("Z ", &g_cfg->world_esp.flZ, -90, 90);
                ImGui::SliderInt("FOV ", &g_cfg->world_esp.flfov, 40, 120);
            }
        }
        ImGui::EndChild();
    }

    if (m_selected_tab == 3) {
        ImGui::BeginChild("player esp ", ImVec2{ 0, 0 }, true);
        {
            ImGui::Checkbox("Teammates", &g_cfg->visuals.m_player_esp.m_teammates);
            ImGui::Checkbox("Bounding box", &g_cfg->visuals.m_player_esp.m_bounding_box);
            ImGui::SameLine();
            ImGui::ColorEdit4("##box color", reinterpret_cast<float*>(&g_cfg->visuals.m_player_esp.m_bounding_box_col), ImGuiColorEditFlags_NoInputs);

            ImGui::Checkbox("Skeleton", &g_cfg->visuals.m_player_esp.m_skeleton);
            ImGui::SameLine();
            ImGui::ColorEdit4("##skeleton color", reinterpret_cast<float*>(&g_cfg->visuals.m_player_esp.m_skeleton_color), ImGuiColorEditFlags_NoInputs);

            ImGui::Checkbox("Off-screen arrows", &g_cfg->visuals.m_player_esp.m_off_arrow);
            ImGui::SameLine();
            ImGui::ColorEdit4("##arrow color", reinterpret_cast<float*>(&g_cfg->visuals.m_player_esp.m_off_arrow_color), ImGuiColorEditFlags_NoInputs);

            ImGui::Checkbox("Health bar", &g_cfg->visuals.m_player_esp.m_health_bar);
            ImGui::Checkbox("Name", &g_cfg->visuals.m_player_esp.m_name);
            ImGui::SameLine();
            ImGui::ColorEdit4("##name color", reinterpret_cast<float*>(&g_cfg->visuals.m_player_esp.m_name_color), ImGuiColorEditFlags_NoInputs);
            ImGui::Checkbox("Weapon text", &g_cfg->visuals.m_player_esp.m_weapon);
            ImGui::SameLine();
            ImGui::ColorEdit4("##weapon text color", reinterpret_cast<float*>(&g_cfg->visuals.m_player_esp.m_weapon_color), ImGuiColorEditFlags_NoInputs);
            ImGui::Checkbox("Weapon icon", &g_cfg->visuals.m_player_esp.m_weapon_icons);
            ImGui::SameLine();
            ImGui::ColorEdit4("##weapon icon color", reinterpret_cast<float*>(&g_cfg->visuals.m_player_esp.m_weapon_icon_col), ImGuiColorEditFlags_NoInputs);

            ImGui::Separator();

            ImGui::Checkbox("Local Glow", &g_cfg->visuals.glow.m_bGlow_local);
            ImGui::SameLine();
            ImGui::ColorEdit4("##glow local col0r", reinterpret_cast<float*>(&g_cfg->visuals.glow.glow_color_local), ImGuiColorEditFlags_NoInputs);
        }
        ImGui::EndChild();
    }

    if (m_selected_tab == 4) {
        ImGui::BeginChild("chams", ImVec2{ 0, 0 }, true);
        {
            const char* chamsMaterials[] = { "Flat", "Illuminate", "Glow" };

            ImGui::Text("Enemy Chams");
            ImGui::Checkbox("Visible", &g_cfg->visuals.chams.m_enemy.m_visible);
            ImGui::SameLine();
            ImGui::ColorEdit4("##Enemy Visible Color", reinterpret_cast<float*>(&g_cfg->visuals.chams.m_enemy.m_visible_color), ImGuiColorEditFlags_NoInputs);
            ImGui::Checkbox("Occluded", &g_cfg->visuals.chams.m_enemy.m_occluded);
            ImGui::SameLine();
            ImGui::ColorEdit4("##Enemy Occluded Color", reinterpret_cast<float*>(&g_cfg->visuals.chams.m_enemy.m_occluded_color), ImGuiColorEditFlags_NoInputs);
            ImGui::Combo("Enemy Material", &g_cfg->visuals.chams.m_enemy.m_type, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));

            ImGui::Separator();

            ImGui::Text("Local Chams");
            ImGui::Checkbox("Local Enabled", &g_cfg->visuals.chams.m_local.m_enabled);
            ImGui::SameLine();
            ImGui::ColorEdit4("##Local Color", reinterpret_cast<float*>(&g_cfg->visuals.chams.m_local.m_color), ImGuiColorEditFlags_NoInputs);
            ImGui::Checkbox("Local Occluded", &g_cfg->visuals.chams.m_local.m_occluded);
            ImGui::SameLine();
            ImGui::ColorEdit4("##Local Occluded Color", reinterpret_cast<float*>(&g_cfg->visuals.chams.m_local.m_occluded_color), ImGuiColorEditFlags_NoInputs);
            ImGui::Combo("Local Material", &g_cfg->visuals.chams.m_local.m_type, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));

            ImGui::Separator();

            ImGui::Text("Weapon Chams");
            ImGui::Checkbox("Weapon Enabled", &g_cfg->visuals.chams.m_weapon.m_enabled);
            ImGui::SameLine();
            ImGui::ColorEdit4("##Weapon Color", reinterpret_cast<float*>(&g_cfg->visuals.chams.m_weapon.m_color), ImGuiColorEditFlags_NoInputs);
            ImGui::Combo("Weapon Material", &g_cfg->visuals.chams.m_weapon.m_type, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
        }
        ImGui::EndChild();
    }

    if (m_selected_tab == 5) {
        ImGui::BeginChild("misc", ImVec2{ 165, 0 }, true);
        {
            const char* removal_items[] = { "Remove scope", "Remove legs", "Remove crosshair", "Remove flash", "Remove smoke", "Remove team intro", "Remove overhead", "Remove aim punch" };
            static const char* sounds[] = { ("flick"), ("bell"), ("mettalic"), ("bubble") };
            static const char* indicators_items[] = { ("Keybinds"), ("Watermark") };

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

            ImGui::Checkbox("Hit Logs", &g_cfg->misc.m_hit_logs);

            ImGui::Checkbox(xorstr_("Hit Sound"), &g_cfg->misc.m_hitsound);

            if (g_cfg->misc.m_hitsound)
                ImGui::Combo(xorstr_("##hit_sound_mode"), &g_cfg->misc.m_hit_sound_mode, sounds, IM_ARRAYSIZE(sounds), sizeof(sounds));                  

            ImGui::Checkbox(xorstr_("tracers"), &g_cfg->world.m_bullet_tracer);

            ImGui::MultiCombo("Indicators", g_cfg->indicators.m_indicators, indicators_items, IM_ARRAYSIZE(indicators_items));
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

                ImGui::Checkbox("Silent strafe", &g_cfg->misc.m_silent_strafe);
            }

            ImGui::Checkbox("auto peek", &g_cfg->misc.m_auto_peek);

            if (g_cfg->misc.m_auto_peek) {
                ImGui::Keybind(xorstr_("Auto Peek Keybind"), &g_cfg->misc.m_override_quick_peek_assistant, &g_cfg->misc.m_override_quick_peek_assistant_style);
            }
        }
        ImGui::EndChild();
    }

    if (m_selected_tab == 6)
    {
        ImGui::BeginGroup();
        {
            ImGui::BeginChild("Skins", ImVec2{ 0, 0 }, true);
            {
               // ImGui::Checkbox(xorstr_("enable knife changer"), &g_cfg->skins.m_knives.m_knife_changer);

                if (g_cfg->skins.m_knives.m_knife_changer)
                {
                    ImGui::Text(xorstr_("knife (ct/t command):"));
                    ImGui::Combo(xorstr_("##knifes"), &g_cfg->skins.m_knives.m_selected,
                        g_skins->m_knives.m_dumped_knife_name.data(), g_skins->m_knives.m_dumped_knife_name.size());
                }

                ImGui::Checkbox(xorstr_("Model changer"), &g_cfg->skins.m_custom_models);

                if (ImGui::Button(xorstr_("Refresh"), ImVec2(ImGui::GetWindowWidth() - 25, 25)))
                    g_model_changer->update_player_models();

                if (!g_model_changer->player_models.empty())
                {

                    std::vector<const char*> model_names;
                    for (auto& mdl : g_model_changer->player_models)
                        model_names.push_back(mdl.name.c_str());

                    ImGui::Spacing();
                    ImGui::Text(xorstr_("Available models:"));

                    int cur_index = g_model_changer->cur_player_model;

                    if (ImGui::Combo(xorstr_("##model_combo"), &cur_index, model_names.data(), static_cast<int>(model_names.size())))
                    {
                        g_model_changer->cur_player_model = cur_index;
                    }
                }
                else
                {
                    ImGui::TextDisabled(xorstr_("No models loaded"));
                }
            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
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