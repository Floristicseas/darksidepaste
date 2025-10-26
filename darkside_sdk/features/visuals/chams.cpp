#include "chams.hpp"

#include <iostream>
#include <stdexcept>

#include "../../darkside.hpp"

#include "../../valve/interfaces/vtables/i_material_system.hpp"

#include "../../valve/interfaces/vtables/i_schema_system.hpp"

struct custom_material_data_t {
    material2_t* m_material;
    material2_t* m_material_z;
};

static custom_material_data_t array_materials[VISUAL_MATERIAL_MAX];

bool chams::chams_t::initialize()
{
    if (this->m_initialized)
        return this->m_initialized;

    // Create materials using the material system
    array_materials[VISUAL_MATERIAL_SOLID] = custom_material_data_t{
        .m_material = CreateMaterial("darkside_solid_visible", false, "csgo_complex.vfx"),
        .m_material_z = CreateMaterial("darkside_solid_occluded", true, "csgo_complex.vfx")
    };

    array_materials[VISUAL_MATERIAL_FLAT] = custom_material_data_t{
        .m_material = CreateMaterial("darkside_flat_visible", false, "csgo_unlitgeneric.vfx"),
        .m_material_z = CreateMaterial("darkside_flat_occluded", true, "csgo_unlitgeneric.vfx")
    };

    array_materials[VISUAL_MATERIAL_GLOW] = custom_material_data_t{
        .m_material = CreateMaterial("darkside_glow_visible", false, "solidcolor.vfx"),
        .m_material_z = CreateMaterial("darkside_glow_occluded", true, "solidcolor.vfx")
    };

    array_materials[VISUAL_MATERIAL_LATEX] = custom_material_data_t{
        .m_material = CreateMaterial("darkside_latex_visible", false, "csgo_effects.vfx"),
        .m_material_z = CreateMaterial("darkside_latex_occluded", true, "csgo_effects.vfx")
    };

    array_materials[VISUAL_MATERIAL_STAR] = custom_material_data_t{
        .m_material = CreateMaterial("darkside_star_visible", false, "csgo_effects.vfx"),
        .m_material_z = CreateMaterial("darkside_star_occluded", true, "csgo_effects.vfx")
    };

    array_materials[VISUAL_MATERIAL_ELECTRIC] = custom_material_data_t{
        .m_material = CreateMaterial("darkside_electric_visible", false, "csgo_complex.vfx"),
        .m_material_z = CreateMaterial("darkside_electric_occluded", true, "csgo_complex.vfx")
    };

    this->m_initialized = true;

    // Verify all materials were created successfully
    for (auto& material : array_materials)
    {
        if (material.m_material == nullptr || material.m_material_z == nullptr)
        {
            this->m_initialized = false;
            break;
        }
    }

    return this->m_initialized;
}

bool chams::chams_t::draw_object(void* animatable_object, void* dx11, material_data_t* arr_material_data, int data_count, void* scene_view, void* scene_layer, void* unk1, void* unk2)
{
    if (!this->m_initialized)
        return false;

    if (!g_ctx->m_local_pawn)
        return false;

    if (arr_material_data == nullptr || arr_material_data->m_scene_animable() == nullptr)
        return false;

    c_base_handle m_owner = arr_material_data->m_scene_animable()->m_owner();

    auto m_entity = g_interfaces->m_entity_system->get_base_entity(m_owner.get_entry_index());
    if (m_entity == nullptr)
        return false;

    c_schema_class_info* m_class_info = m_entity->get_schema_class_info();
    if (m_class_info == nullptr)
        return false;

    if (strcmp(m_class_info->get_name(), "C_CSPlayerPawn") == 0)
    {
        auto m_pawn = reinterpret_cast<c_cs_player_pawn*>(m_entity);
        if (m_pawn == nullptr)
            return false;

        bool bIsLocalPlayer = (m_pawn == g_ctx->m_local_pawn);

        if (!g_cfg->visuals.chams.m_local.m_enabled && bIsLocalPlayer)
            return false;

        if (!bIsLocalPlayer && !m_pawn->IsEnemy(g_ctx->m_local_pawn))
            return false;

        return this->override_material(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
    }

    // Handle weapons
    if (g_cfg->visuals.chams.m_weapon.m_enabled && m_entity->is_weapon())
    {
        auto pOwnerEntity = g_interfaces->m_entity_system->get_base_entity(m_entity->get_handle().get_entry_index());
        if (pOwnerEntity && pOwnerEntity == g_ctx->m_local_pawn)
            return this->override_material(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
    }

    // Handle view models
    if (g_cfg->visuals.chams.m_weapon.m_enabled && m_entity->is_view_model())
    {
        auto pOwnerEntity = g_interfaces->m_entity_system->get_base_entity(m_entity->get_handle().get_entry_index());
        if (pOwnerEntity && pOwnerEntity == g_ctx->m_local_pawn)
            return this->override_material(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
    }

    return false;
}

material2_t* chams::chams_t::CreateMaterial(const char* material_name, bool ignore_z, const char* shader_name)
{
    if (!g_material_system)
        return nullptr;

    // Create material data
    material_data_t* material_data = new material_data_t();
    if (!material_data)
        return nullptr;

    // Set shader type
    material_data->set_shader_type(shader_name);

    // Set material functions based on type and ignore_z
    if (ignore_z)
    {
        material_data->set_material_function("F_IGNOREZ", 1);
        material_data->set_material_function("F_DISABLE_Z_WRITE", 1);
        material_data->set_material_function("F_DISABLE_Z_BUFFERING", 1);
    }
    else
    {
        material_data->set_material_function("F_IGNOREZ", 0);
        material_data->set_material_function("F_DISABLE_Z_WRITE", 0);
        material_data->set_material_function("F_DISABLE_Z_BUFFERING", 0);
    }

    // Common material functions
    material_data->set_material_function("F_TRANSLUCENT", 1);
    material_data->set_material_function("F_PAINT_VERTEX_COLORS", 1);

    // Additional functions based on shader type
    if (strstr(shader_name, "unlitgeneric"))
    {
        material_data->set_material_function("F_BLEND_MODE", 1);
    }
    else if (strstr(shader_name, "solidcolor"))
    {
        material_data->set_material_function("F_SELF_ILLUM", 1);
    }
    else if (strstr(shader_name, "effects"))
    {
        material_data->set_material_function("F_ADDITIVE_BLEND", 1);
    }

    // Create the material
    material2_t* material = g_material_system->create_material(material_name, material_data);

    // Clean up
    delete material_data;

    return material;
}

bool chams::chams_t::override_material(void* animatable_object, void* dx11, material_data_t* arr_material_data, int data_count, void* scene_view, void* scene_layer, void* unk1, void* unk2)
{
    static auto original = hooks::object::drawobject.get_original<decltype(&hooks::object::hk_draw_object)>();

    const custom_material_data_t customMaterialEnemy = array_materials[g_cfg->visuals.chams.m_enemy.m_type];
    const custom_material_data_t customMaterialLocal = array_materials[g_cfg->visuals.chams.m_local.m_type];
    const custom_material_data_t customMaterialWeap = array_materials[g_cfg->visuals.chams.m_weapon.m_type];

    auto pEntity = g_interfaces->m_entity_system->get_base_entity(arr_material_data->m_scene_animable()->m_owner().get_entry_index());
    if (pEntity == nullptr)
        return false;

    c_schema_class_info* pClassInfo = pEntity->get_schema_class_info();
    if (pClassInfo == nullptr)
        return false;

    if (strcmp(pClassInfo->get_name(), "C_CSPlayerPawn") == 0)
    {
        auto pPlayerPawn = reinterpret_cast<c_cs_player_pawn*>(pEntity);
        if (pPlayerPawn == nullptr)
            return false;

        bool bIsLocalPlayer = (pPlayerPawn == g_ctx->m_local_pawn);

        if (!bIsLocalPlayer)
        {
            if (g_cfg->visuals.chams.m_enemy.m_occluded)
            {
                arr_material_data->m_material() = customMaterialEnemy.m_material_z;
                arr_material_data->m_color() = g_cfg->visuals.chams.m_enemy.m_occluded_color;
                g_material_system->set_color(arr_material_data, g_cfg->visuals.chams.m_enemy.m_occluded_color);

                original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
            }

            if (g_cfg->visuals.chams.m_enemy.m_visible)
            {
                arr_material_data->m_material() = customMaterialEnemy.m_material;
                arr_material_data->m_color() = g_cfg->visuals.chams.m_enemy.m_visible_color;
                g_material_system->set_color(arr_material_data, g_cfg->visuals.chams.m_enemy.m_visible_color);

                original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
            }
        }
        else if (bIsLocalPlayer && g_cfg->visuals.chams.m_local.m_enabled)
        {
            if (g_cfg->visuals.chams.m_local.m_occluded)
            {
                arr_material_data->m_material() = customMaterialLocal.m_material_z;
                arr_material_data->m_color() = g_cfg->visuals.chams.m_local.m_occluded_color;
                g_material_system->set_color(arr_material_data, g_cfg->visuals.chams.m_local.m_occluded_color);

                original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
            }

            arr_material_data->m_material() = customMaterialLocal.m_material;
            arr_material_data->m_color() = g_cfg->visuals.chams.m_local.m_color;
            g_material_system->set_color(arr_material_data, g_cfg->visuals.chams.m_local.m_color);

            original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
        }
    }
    else if (g_cfg->visuals.chams.m_weapon.m_enabled && pEntity->is_weapon())
    {
        auto pOwnerEntity = g_interfaces->m_entity_system->get_base_entity(pEntity->get_handle().get_entry_index());
        if (pOwnerEntity && pOwnerEntity == g_ctx->m_local_pawn)
        {
            arr_material_data->m_material() = customMaterialWeap.m_material;
            arr_material_data->m_color() = g_cfg->visuals.chams.m_weapon.m_color;
            g_material_system->set_color(arr_material_data, g_cfg->visuals.chams.m_weapon.m_color);
        }
    }
    else if (g_cfg->visuals.chams.m_weapon.m_enabled && pEntity->is_view_model())
    {
        auto pOwnerEntity = g_interfaces->m_entity_system->get_base_entity(pEntity->get_handle().get_entry_index());
        if (pOwnerEntity && pOwnerEntity == g_ctx->m_local_pawn)
        {
            arr_material_data->m_material() = customMaterialWeap.m_material;
            arr_material_data->m_color() = g_cfg->visuals.chams.m_weapon.m_color;
            g_material_system->set_color(arr_material_data, g_cfg->visuals.chams.m_weapon.m_color);
        }
    }

    original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);

    return true;
}