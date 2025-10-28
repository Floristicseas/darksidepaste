#include "config_system.hpp"

#include "../../features/skins/skins.hpp"

#include <filesystem>
#include <fstream>

#include "../../sdk/includes/json/json.h"

void c_config_system::setup_values()
{
    // Rage Bot
    push_item(&g_cfg->rage_bot.m_enabled, "rage_bot", "enabled", g_cfg->rage_bot.m_enabled);
    push_item(&g_cfg->rage_bot.m_silent, "rage_bot", "silent", g_cfg->rage_bot.m_silent);
    push_item(&g_cfg->rage_bot.m_static_scale, "rage_bot", "static_scale", g_cfg->rage_bot.m_static_scale);
    push_item(&g_cfg->rage_bot.m_minimum_damage, "rage_bot", "minimum_damage", g_cfg->rage_bot.m_minimum_damage);
    push_item(&g_cfg->rage_bot.m_minimum_damage_override, "rage_bot", "minimum_damage_override", g_cfg->rage_bot.m_minimum_damage_override);
    push_item(&g_cfg->rage_bot.m_hit_chance, "rage_bot", "hit_chance", g_cfg->rage_bot.m_hit_chance);
    push_item(&g_cfg->rage_bot.m_auto_fire, "rage_bot", "auto_fire", g_cfg->rage_bot.m_auto_fire);
    push_item(&g_cfg->rage_bot.m_auto_stop, "rage_bot", "auto_stop", g_cfg->rage_bot.m_auto_stop);
    push_item(&g_cfg->rage_bot.m_override_damage_key_bind, "rage_bot", "override_damage_keybind", g_cfg->rage_bot.m_override_damage_key_bind);
    push_item(&g_cfg->rage_bot.m_override_damage_key_bind_style, "rage_bot", "override_damage_keybind_style", g_cfg->rage_bot.m_override_damage_key_bind_style);

    for (auto hitbox = 0; hitbox < 6; hitbox++) {
        push_item(&g_cfg->rage_bot.m_hitboxes[hitbox], "rage_bot", std::format("hitbox_{0}", hitbox), g_cfg->rage_bot.m_hitboxes[hitbox]);
        push_item(&g_cfg->rage_bot.m_multi_points[hitbox], "rage_bot", std::format("multi_points_{0}", hitbox), g_cfg->rage_bot.m_multi_points[hitbox]);
        push_item(&g_cfg->rage_bot.m_hitbox_scale[hitbox], "rage_bot", std::format("multi_points_scale_{0}", hitbox), g_cfg->rage_bot.m_hitbox_scale[hitbox]);
    }

    // Anti-Hit
    push_item(&g_cfg->anti_hit.m_enabled, "anti_aim", "enabled", g_cfg->anti_hit.m_enabled);
    push_item(&g_cfg->anti_hit.m_at_target, "anti_aim", "at_target", g_cfg->anti_hit.m_at_target);
    push_item(&g_cfg->anti_hit.m_override_pitch, "anti_aim", "override_pitch", g_cfg->anti_hit.m_override_pitch);
    push_item(&g_cfg->anti_hit.m_pitch_amount, "anti_aim", "pitch_amount", g_cfg->anti_hit.m_pitch_amount);
    push_item(&g_cfg->anti_hit.m_override_yaw, "anti_aim", "override_yaw", g_cfg->anti_hit.m_override_yaw);
    push_item(&g_cfg->anti_hit.m_yaw_amount, "anti_aim", "yaw_amount", g_cfg->anti_hit.m_yaw_amount);
    push_item(&g_cfg->anti_hit.m_jitter_amount, "anti_aim", "jitter_amount", g_cfg->anti_hit.m_jitter_amount);

    // Visuals - Player ESP
    push_item(&g_cfg->visuals.m_player_esp.m_bounding_box, "player_esp", "bounding_box", g_cfg->visuals.m_player_esp.m_bounding_box);
    push_item(&g_cfg->visuals.m_player_esp.m_health_bar, "player_esp", "health_bar", g_cfg->visuals.m_player_esp.m_health_bar);
    push_item(&g_cfg->visuals.m_player_esp.m_name, "player_esp", "name", g_cfg->visuals.m_player_esp.m_name);
    push_item(&g_cfg->visuals.m_player_esp.m_weapon, "player_esp", "weapon", g_cfg->visuals.m_player_esp.m_weapon);
    push_item(&g_cfg->visuals.m_player_esp.m_flags, "player_esp", "flags", g_cfg->visuals.m_player_esp.m_flags);
    push_item(&g_cfg->visuals.m_player_esp.m_teammates, "player_esp", "teammates", g_cfg->visuals.m_player_esp.m_teammates);
    push_item(&g_cfg->visuals.m_player_esp.m_weapon_icons, "player_esp", "weapon_icons", g_cfg->visuals.m_player_esp.m_weapon_icons);
    push_item(&g_cfg->visuals.m_player_esp.m_name_color, "player_esp", "name_color", g_cfg->visuals.m_player_esp.m_name_color);

    // Visuals - Other
    push_item(&g_cfg->visuals.debug_icon, "visuals", "debug_icon", g_cfg->visuals.debug_icon);
    //push_item(&g_cfg->visuals.m_remove_punch, "visuals", "remove_punch", g_cfg->visuals.m_remove_punch);
    push_item(&g_cfg->visuals.glow.m_bGlow, "visuals", "glow", g_cfg->visuals.glow.m_bGlow);
    push_item(&g_cfg->visuals.glow.m_bGlowOnRagdoll, "visuals", "glow_on_ragdoll", g_cfg->visuals.glow.m_bGlowOnRagdoll);

    // World ESP
    push_item(&g_cfg->world_esp.m_enable_thirdperson, "thirdperson", "enabled", g_cfg->world_esp.m_enable_thirdperson);
    push_item(&g_cfg->world_esp.m_enable_override_fov, "thirdperson", "enable_override_fov", g_cfg->world_esp.m_enable_override_fov);
    push_item(&g_cfg->world_esp.m_distance, "thirdperson", "distance", g_cfg->world_esp.m_distance);
    push_item(&g_cfg->world_esp.viewchanger, "thirdperson", "viewchanger", g_cfg->world_esp.viewchanger);
    push_item(&g_cfg->world_esp.m_override_fov, "thirdperson", "override_fov", g_cfg->world_esp.m_override_fov);
    push_item(&g_cfg->world_esp.custom_crosshair, "thirdperson", "custom_crosshair", g_cfg->world_esp.custom_crosshair);
    push_item(&g_cfg->world_esp.enable_aspect, "thirdperson", "enable_aspect", g_cfg->world_esp.enable_aspect);
    push_item(&g_cfg->world_esp.scope_lines_initial_pos, "thirdperson", "scope_lines_initial_pos", g_cfg->world_esp.scope_lines_initial_pos);
    push_item(&g_cfg->world_esp.scope_lines_offset, "thirdperson", "scope_lines_offset", g_cfg->world_esp.scope_lines_offset);
    push_item(&g_cfg->world_esp.scope_lines_fade_speed, "thirdperson", "scope_lines_fade_speed", g_cfg->world_esp.scope_lines_fade_speed);
    push_item(&g_cfg->world_esp.scope_lines_color, "thirdperson", "scope_lines_color", g_cfg->world_esp.scope_lines_color);
    push_item(&g_cfg->world_esp.flX, "thirdperson", "flX", g_cfg->world_esp.flX);
    push_item(&g_cfg->world_esp.flY, "thirdperson", "flY", g_cfg->world_esp.flY);
    push_item(&g_cfg->world_esp.flZ, "thirdperson", "flZ", g_cfg->world_esp.flZ);
    push_item(&g_cfg->world_esp.flXrotation, "thirdperson", "flXrotation", g_cfg->world_esp.flXrotation);
    push_item(&g_cfg->world_esp.flYrotation, "thirdperson", "flYrotation", g_cfg->world_esp.flYrotation);
    push_item(&g_cfg->world_esp.flZrotation, "thirdperson", "flZrotation", g_cfg->world_esp.flZrotation);
    push_item(&g_cfg->world_esp.flfov, "thirdperson", "flfov", g_cfg->world_esp.flfov);
    push_item(&g_cfg->world_esp.m_thirdperson_key_bind, "thirdperson", "thirdperson_keybind", g_cfg->world_esp.m_thirdperson_key_bind);
    push_item(&g_cfg->world_esp.m_thirdperson_key_bind_style, "thirdperson", "thirdperson_keybind_style", g_cfg->world_esp.m_thirdperson_key_bind_style);
    push_item(&g_cfg->world_esp.m_aspect_ratio, "thirdperson", "aspect_ratio", g_cfg->world_esp.m_aspect_ratio);

    // World Modulation
    push_item(&g_cfg->world.m_exposure, "world", "exposure", g_cfg->world.m_exposure);
    push_item(&g_cfg->world.m_lighting, "world", "lighting", g_cfg->world.m_lighting);
    push_item(&g_cfg->world.m_render_fog, "world", "render_fog", g_cfg->world.m_render_fog);
    push_item(&g_cfg->world.m_sky, "world", "sky_color", g_cfg->world.m_sky);
    push_item(&g_cfg->world.m_sky_clouds, "world", "sky_clouds_color", g_cfg->world.m_sky_clouds);
    push_item(&g_cfg->world.m_wall, "world", "wall_color", g_cfg->world.m_wall);

    // Misc
    push_item(&g_cfg->misc.m_auto_peek, "misc", "auto_peek", g_cfg->misc.m_auto_peek);
    push_item(&g_cfg->misc.m_auto_peek_return, "misc", "auto_peek_return", g_cfg->misc.m_auto_peek_return);
    push_item(&g_cfg->misc.m_quick_stop, "misc", "quick_stop", g_cfg->misc.m_quick_stop);
    push_item(&g_cfg->misc.m_silent_strafe, "misc", "silent_strafe", g_cfg->misc.m_silent_strafe);
    push_item(&g_cfg->misc.m_auto_strafe_type, "misc", "auto_strafe_type", g_cfg->misc.m_auto_strafe_type);
    push_item(&g_cfg->misc.m_directional_type, "misc", "directional_type", g_cfg->misc.m_directional_type);
    push_item(&g_cfg->misc.m_hop, "misc", "bhop", g_cfg->misc.m_hop);
    push_item(&g_cfg->misc.m_auto_strafe, "misc", "auto_strafe", g_cfg->misc.m_auto_strafe);
    push_item(&g_cfg->misc.m_field_of_view, "misc", "field_of_view", g_cfg->misc.m_field_of_view);
    push_item(&g_cfg->misc.m_strafe_smooth, "misc", "strafe_smooth", g_cfg->misc.m_strafe_smooth);
    push_item(&g_cfg->misc.m_quick_peek_assistant_type, "misc", "quick_peek_assistant_type", g_cfg->misc.m_quick_peek_assistant_type);
    push_item(&g_cfg->misc.m_override_quick_peek_assistant, "misc", "override_quick_peek_assistant", g_cfg->misc.m_override_quick_peek_assistant);
    push_item(&g_cfg->misc.m_override_quick_peek_assistant_style, "misc", "override_quick_peek_assistant_style", g_cfg->misc.m_override_quick_peek_assistant_style);
    push_item(&g_cfg->misc.hitmark_clr, "misc", "hitmark_color", g_cfg->misc.hitmark_clr);

    const char* removal_names[] = { "scope", "legs", "crosshair", "flash", "smoke", "intro", "overhead_local" };
    for (int i = 0; i < 7; i++) {
        push_item(&g_cfg->misc.m_removals[i], "misc", std::format("removal_{}", removal_names[i]), g_cfg->misc.m_removals[i]);
    }

    // Skins
    /*push_item(&g_cfg->skins.m_agents.m_selected_ct, "agent_changer", "selected_ct", g_cfg->skins.m_agents.m_selected_ct);
    push_item(&g_cfg->skins.m_agents.m_selected, "agent_changer", "selected_t", g_cfg->skins.m_agents.m_selected);
    push_item(&g_cfg->skins.m_knives.m_selected, "knife_changer", "selected", g_cfg->skins.m_knives.m_selected);
    push_item(&g_cfg->skins.m_gloves.m_selected, "glove_changer", "selected", g_cfg->skins.m_gloves.m_selected);
    push_item(&g_cfg->skins.m_selected_weapon, "skin_changer", "selected_weapon", g_cfg->skins.m_selected_weapon);
    push_item(&g_cfg->skins.m_selected_type, "skin_changer", "selected_type", g_cfg->skins.m_selected_type);*/

    static std::vector<const char*> weapons{};

   /* for (auto weapon = 0; weapon < weapons.size(); weapon++) {
        push_item(&g_cfg->skins.m_skin_settings[weapon].m_paint_kit, "skin_changer", std::format("paint_kit_{0}", weapon), g_cfg->skins.m_skin_settings[weapon].m_paint_kit);
        push_item(&g_cfg->skins.m_skin_settings[weapon].m_item_def_index, "skin_changer", std::format("item_def_index_{0}", weapon), g_cfg->skins.m_skin_settings[weapon].m_item_def_index);
        push_item(&g_cfg->skins.m_skin_settings[weapon].m_prev_skin, "skin_changer", std::format("prev_skin_{0}", weapon), g_cfg->skins.m_skin_settings[weapon].m_prev_skin);
    }*/
}

void c_config_system::push_item(bool* pointer, std::string category, std::string name, bool def_value)
{
    this->booleans.push_back({ pointer, category, name, def_value });
}

void c_config_system::push_item(int* pointer, std::string category, std::string name, int def_value)
{
    this->ints.push_back({ pointer, category, name, def_value });
}

void c_config_system::push_item(float* pointer, std::string category, std::string name, float def_value)
{
    this->floats.push_back({ pointer, category, name, def_value });
}

void c_config_system::push_item(c_color* pointer, std::string category, std::string name, c_color def_value)
{
    this->colors_rgb.push_back({ pointer, category, name, def_value });
}

inline bool FileExists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

using nlohmann::json;
void c_config_system::read(std::string path) {
    path = "c:\\airflow\\configs\\" + path + ".cfg";

    if (!FileExists(path))
        return;

    std::ifstream reader(path);

    json data;
    reader >> data;
    reader.close();

    auto sanity_checks = [](json& d, std::string& c, std::string& n) -> bool {
        return d.find(c) != d.end() && d[c].find(n) != d[c].end();
        };

    for (auto& b : booleans)
        if (sanity_checks(data, b.category, b.name))
            *b.pointer = data[b.category][b.name].get<bool>();

    for (auto& i : ints)
        if (sanity_checks(data, i.category, i.name))
            *i.pointer = data[i.category][i.name].get<int>();

    for (auto& f : floats)
        if (sanity_checks(data, f.category, f.name))
            *f.pointer = data[f.category][f.name].get<float>();

    for (auto& c : colors_rgb) {
        if (sanity_checks(data, c.category, c.name)) {
            auto elem = data[c.category][c.name];
            c.pointer->r = elem["r"].get<float>();
            c.pointer->g = elem["g"].get<float>();
            c.pointer->b = elem["b"].get<float>();
            c.pointer->a = elem["a"].get<float>();
        }
    }

    for (auto& i : keybinds)
        if (sanity_checks(data, i.category, i.name))
            *i.pointer = data[i.category][i.name].get<int>();
}

void c_config_system::save(std::string path) {
    path = "c:\\airflow\\configs\\" + path + ".cfg";

    json ret_data;
    auto sanity_checks = [&](std::string category) -> void {
        if (ret_data.find(category) == ret_data.end())
            ret_data[category] = json();
        };

    for (auto& b : booleans) {
        sanity_checks(b.category);
        ret_data[b.category][b.name] = *b.pointer;
    }

    for (auto& i : ints) {
        sanity_checks(i.category);
        ret_data[i.category][i.name] = *i.pointer;
    }

    for (auto& f : floats) {
        sanity_checks(f.category);
        ret_data[f.category][f.name] = *f.pointer;
    }

    for (auto& c : colors_rgb) {
        sanity_checks(c.category);
        json d;
        c_color clr = *c.pointer;
        d["r"] = clr.r;
        d["g"] = clr.g;
        d["b"] = clr.b;
        d["a"] = clr.a;
        ret_data[c.category][c.name] = d;
    }

    for (auto& i : keybinds) {
        sanity_checks(i.category);
        ret_data[i.category][i.name] = *i.pointer;
    }

    std::ofstream reader(path);
    reader.clear();
    reader << std::setw(4) << ret_data << std::endl;
    reader.close();
}

void c_config_system::refresh() {
    config_files.clear();

    for (const auto& p : std::filesystem::recursive_directory_iterator("c:\\airflow\\configs\\")) {
        if (!std::filesystem::is_directory(p) &&
            p.path().extension().string() == ".cfg") {
            auto file = p.path().filename().string();
            config_files.push_back(file.substr(0, file.size() - 4));
        }
    }
}

void c_config_system::remove(std::string path) {
    path = "c:\\airflow\\configs\\" + path + ".cfg";

    std::filesystem::remove(path);
    refresh();
}

void c_config_system::reset() {
    for (auto& b : booleans)
        *b.pointer = b.default_value;
    for (auto& i : ints)
        *i.pointer = i.default_value;
    for (auto& c : colors_rgb)
        *c.pointer = c.default_value;
    for (auto& f : floats)
        *f.pointer = f.default_value;
    for (auto& i : keybinds)
        *i.pointer = i.default_value;
}

static std::string selected_cfg = "";
bool confirm_opened = false;
bool confirmed = false;
bool delete_cfg = false;

void confirm_save(bool deleting)
{
    confirm_opened = true;
    delete_cfg = deleting;
}

void final_save()
{
    if (delete_cfg)
        g_config_system->remove(selected_cfg);
    else
        g_config_system->save(selected_cfg);

    confirm_opened = false;
    confirmed = true;
}

void no_confirm()
{
    confirm_opened = false;
    confirmed = false;
}

void window()
{
    if (!confirm_opened)
        return;

    ImGui::Begin("confirmation", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    ImGui::SetWindowSize(ImVec2(240, 145));
    ImGui::SetCursorPosY(39);

    if (confirm_opened)
        delete_cfg ? ImGui::Text("are you sure want to delete this config?") : ImGui::Text("are you sure want to save this config?");

    ImGui::SetCursorPosX(4);
    ImGui::SetCursorPosY(61);

    if (confirm_opened && ImGui::Button(delete_cfg ? "delete" : "save"))
        final_save();

    ImGui::SetCursorPosY(86);
    ImGui::SetCursorPosX(4);

    if (confirm_opened && ImGui::Button("cancel"))
        no_confirm();

    ImGui::End();
}

void c_config_system::menu() {
    refresh();

    static char cfg_name[26];

    if (ImGui::BeginChild("Settings")) {
        ImGui::ListBoxHeader("##cfglist", ImVec2(-1, -205));

        for (auto cfg : config_files)
            if (ImGui::Selectable(cfg.c_str(), cfg == selected_cfg))
                selected_cfg = cfg;

        ImGui::ListBoxFooter();
        ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 193);
        if (ImGui::InputText(" ", cfg_name, 26)) selected_cfg = std::string(cfg_name);
        if (std::string(cfg_name) != "")
        {
            ImGui::SetCursorPosX(4);
            if (ImGui::Button("create")) {

                if (!selected_cfg.empty())
                    save(selected_cfg);

                refresh();
            }
        }
        ImGui::SetCursorPosX(4);
        if (ImGui::Button("load")) {
            if (!selected_cfg.empty())
                read(selected_cfg);

            refresh();
        }
        ImGui::SetCursorPosX(4);
        if (ImGui::Button("save")) {
            if (!selected_cfg.empty())
                confirm_save(false);

            refresh();
        }

        window();

        ImGui::SetCursorPosX(4);
        if (ImGui::Button("delete")) {
            if (!selected_cfg.empty())
                confirm_save(true);

            refresh();
        }
        ImGui::SetCursorPosX(4);
        if (ImGui::Button("reset"))
            reset();

        ImGui::SetCursorPosX(4);
        if (ImGui::Button("refresh"))
            refresh();

        ImGui::EndChild();
    }
}