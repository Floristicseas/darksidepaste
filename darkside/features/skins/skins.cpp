#include "skins.hpp"
#include "../../darkside.hpp"
#include "../../valve/interfaces/vtables/resourcesystem.hpp"

#include <iostream>
#include <filesystem>
#include <algorithm>

void c_skins::dump_items()
{
    auto item_schema = g_interfaces->m_client->get_econ_item_system()->get_econ_item_schema();
    if (!item_schema)
        return;

    const auto& vec_items = item_schema->m_sorted_item_definition_map;

    for (const auto& it : vec_items)
    {
        c_econ_item_definition* item = it.m_value;
        if (!item)
            continue;

        const bool is_agent = item->is_agent();
        const bool is_knife = item->is_knife(true);

        const char* item_base_name = item->m_item_base_name;
        if (!item_base_name || item_base_name[0] == '\0')
            continue;

        if (is_agent)
        {
            m_agents.m_dumped_agent_models.emplace_back(item->get_model_name());
            m_agents.m_dumped_agent_name.emplace_back(g_interfaces->m_localize->find_key(item->m_item_base_name));
        }

        if (is_knife)
        {
            m_knives.m_dumped_knife_models.emplace_back(item->get_model_name());
            m_knives.m_dumped_knife_name.emplace_back(g_interfaces->m_localize->find_key(item->m_item_base_name));
        }
    }
}

void c_model_changer::update_player_models()
{
    player_models.clear();

    const std::string models_path = parser::GetDirectory() + xorstr_("csgo\\characters\\models\\");

    if (!std::filesystem::exists(models_path) || !std::filesystem::is_directory(models_path))
    {
        std::cout << "[ModelChanger] Models folder not found: " << models_path << std::endl;
        return;
    }

    try
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(models_path))
        {
            if (!entry.is_regular_file())
                continue;

            std::string file_name = entry.path().filename().string();

            if (file_name.size() < 6)
                continue;

            if (file_name.substr(file_name.size() - 7) != xorstr_(".vmdl_c"))
                continue;

            if (file_name.find(xorstr_("arm")) != std::string::npos)
                continue;

            std::string model_path = entry.path().string();
            model_path = model_path.substr(0, model_path.size() - 2);
            std::replace(model_path.begin(), model_path.end(), '\\', '/');

            size_t pos = model_path.find(xorstr_("characters/models/"));
            if (pos != std::string::npos)
                model_path = model_path.substr(pos);

            Model_t pModel;
            pModel.name = file_name.substr(0, file_name.size() - 8);
            pModel.path = model_path;

            player_models.push_back(pModel);
        }

        std::cout << "[ModelChanger] Loaded " << player_models.size() << " player models." << std::endl;
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "[ModelChanger] Filesystem error: " << e.what() << std::endl;
    }
}

void c_model_changer::set_player_model()
{
    if (!g_interfaces->m_engine->is_in_game() || !g_interfaces->m_engine->is_connected())
        return;

    if (!g_ctx->m_local_pawn || !g_ctx->m_local_pawn->is_alive())
        return;

    if (player_models.empty())
        return;

    if (cur_player_model == ~1U)
        return;

    if (cur_player_model >= player_models.size())
        return;

    static uint32_t last_applied_model_index = ~1U;
    static auto last_apply_time = std::chrono::steady_clock::now();

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_apply_time).count();

    if (elapsed < 5 && cur_player_model == last_applied_model_index)
        return;

    Model_t pModel = player_models[cur_player_model];
    const char* model_path = pModel.path.c_str();

    if (!model_path || strlen(model_path) < 5)
        return;

    try
    {
        g_interfaces->ResourceSystem->PreCache(model_path);
        g_ctx->m_local_pawn->SetModel(model_path);

        last_applied_model_index = cur_player_model;
        last_apply_time = now;

        std::cout << "[ModelChanger] Applied model: " << model_path << std::endl;
    }
    catch (...)
    {
        std::cerr << "[ModelChanger] Failed to apply model: " << model_path << std::endl;
    }
}

void c_skins::agent_changer(int stage)
{
   
}

void c_skins::knife_changer(int stage)
{
    if (!g_ctx->m_local_pawn || m_knives.m_dumped_knife_models.empty())
        return;

    c_player_weapon_service* weapon_service = g_ctx->m_local_pawn->m_weapon_services();
    if (!weapon_service)
        return;

    c_cs_player_view_model_service* view_model_service = g_ctx->m_local_pawn->m_view_model_services();
    if (!view_model_service)
        return;

    c_base_view_model* view_model = reinterpret_cast<c_base_view_model*>(g_interfaces->m_entity_system->get_base_entity(view_model_service->m_view_model().get_entry_index()));
    if (!view_model)
        return;

    c_network_utl_vector<c_base_handle>& my_weapons = weapon_service->m_weapon();

    for (auto i = 0; i < my_weapons.m_size; i++)
    {
        c_econ_entity* weapon = reinterpret_cast<c_econ_entity*>(g_interfaces->m_entity_system->get_base_entity(my_weapons.m_elements[i].get_entry_index()));
        if (!weapon)
            continue;

        c_attribute_container* attribute_manager = weapon->m_attribute_manager();
        if (!attribute_manager)
            continue;

        c_econ_item_view* item = attribute_manager->m_item();
        if (!item)
            continue;

        c_econ_item_definition* item_defenition = item->get_static_data();
        if (!item_defenition)
            continue;

        auto weapon_scene = weapon->m_scene_node();
        if (!weapon_scene)
            continue;

        auto view_model_scene = view_model->m_scene_node();
        if (!view_model_scene)
            continue;

        auto item_schema = g_interfaces->m_client->get_econ_item_system()->get_econ_item_schema();
        if (!item_schema)
            return;

        if (!g_cfg->skins.m_knives.m_knife_changer)
            return;

        bool is_knife = item_defenition->is_knife(false);

        int current_weapon = get_skin_config(item->m_defenition_index());
        if (!is_knife && current_weapon == CONFIG_UNKNOWN)
            continue;

        if (is_knife && stage == 6)
        {
            int selectedKnife = g_cfg->skins.m_knives.m_selected;

            weapon->set_model(m_knives.m_dumped_knife_models.at(selectedKnife));
            item->m_defenition_index() = m_knives.m_knife_idxs.at(selectedKnife);

            if (view_model->m_weapon() == weapon->get_handle())
                view_model->set_model(m_knives.m_dumped_knife_models.at(selectedKnife));
        }
    }
}