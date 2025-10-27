#include "chams.hpp"

#include <iostream>
#include <stdexcept>

#include "../../darkside.hpp"

#include "../../valve/interfaces/vtables/i_material_system.hpp"

#include "../../valve/interfaces/vtables/shit.hpp"



struct custom_material_data_t {
    material2_t* m_material;
    material2_t* m_material_z;
};

static custom_material_data_t array_materials[VISUAL_MATERIAL_MAX];

bool chams::chams_t::initialize()
{
    if (this->m_initialized)
        return this->m_initialized;

    array_materials[VISUAL_MATERIAL_SOLID] = custom_material_data_t{
        .m_material = CreateMaterial("materials/dev/primary_white.vmat", szVMatBufferSolidVisible),
        .m_material_z = CreateMaterial("materials/dev/primary_white.vmat", szVMatBufferSolidInvisible)
    };

    array_materials[VISUAL_MATERIAL_FLAT] = custom_material_data_t{
        .m_material = CreateMaterial("materials/dev/primary_white.vmat", szVMatBufferWhiteVisible),
        .m_material_z = CreateMaterial("materials/dev/primary_white.vmat", szVMatBufferWhiteInvisible)
    };

    array_materials[VISUAL_MATERIAL_GLOW] = custom_material_data_t{
        .m_material = CreateMaterial("materials/dev/glow.vmat", szVMatBufferIlluminateVisible),
        .m_material_z = CreateMaterial("materials/dev/glow_invisible.vmat", szVMatBufferIlluminateInvisible)
    };

    array_materials[VISUAL_MATERIAL_LATEX] = custom_material_data_t{
        .m_material = CreateMaterial("materials/dev/primary_white.vmat", szVMatBufferLatexVisible),
        .m_material_z = CreateMaterial("materials/dev/primary_white.vmat", szVMatBufferLatexInvisible)
    };

    array_materials[VISUAL_MATERIAL_STAR] = custom_material_data_t{
        .m_material = CreateMaterial("materials/dev/star.vmat", szVMatBufferStarVisible),
        .m_material_z = CreateMaterial("materials/dev/star_invisible.vmat", szVMatBufferStarInvisible)
    };

    array_materials[VISUAL_MATERIAL_ELECTRIC] = custom_material_data_t{
        .m_material = CreateMaterial("materials/dev/electric.vmat", szVMatBufferElectricVisible),
        .m_material_z = CreateMaterial("materials/dev/electric_invisible.vmat", szVMatBufferElectricInvisible)
    };

    this->m_initialized = true;

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

    if (g_cfg->visuals.chams.m_weapon.m_enabled && m_entity->is_weapon())
    {
        auto pOwnerEntity = g_interfaces->m_entity_system->get_base_entity(m_entity->get_handle().get_entry_index());
        if (pOwnerEntity && pOwnerEntity == g_ctx->m_local_pawn)
            return this->override_material(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
    }


    if (g_cfg->visuals.chams.m_weapon.m_enabled && m_entity->is_view_model())
    {
        auto pOwnerEntity = g_interfaces->m_entity_system->get_base_entity(m_entity->get_handle().get_entry_index());
        if (pOwnerEntity && pOwnerEntity == g_ctx->m_local_pawn)
            return this->override_material(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
    }

    return false;
}


material2_t* chams::chams_t::CreateMaterial(const char* szMaterialName, const char szVmatBuffer[])
{
    CKeyValues3* pKeyValues3 = CKeyValues3::CreateMaterialResource();
    if (!pKeyValues3)
        return nullptr;

    pKeyValues3->LoadFromBuffer(szVmatBuffer);

    material2_t** pCustomMaterial = nullptr;

    g_interfaces->fnCreateMaterial(nullptr, &pCustomMaterial, szMaterialName, pKeyValues3, 0, 1);

    return pCustomMaterial ? *pCustomMaterial : nullptr;
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
                g_interfaces->m_mat_sys->set_color(arr_material_data, g_cfg->visuals.chams.m_enemy.m_occluded_color);

                original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
            }

            if (g_cfg->visuals.chams.m_enemy.m_visible)
            {
                arr_material_data->m_material() = customMaterialEnemy.m_material;
                arr_material_data->m_color() = g_cfg->visuals.chams.m_enemy.m_visible_color;
                g_interfaces->m_mat_sys->set_color(arr_material_data, g_cfg->visuals.chams.m_enemy.m_visible_color);

                original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);
            }
        }
        else if (bIsLocalPlayer && g_cfg->visuals.chams.m_local.m_enabled)
        {
            arr_material_data->m_material() = customMaterialLocal.m_material_z;
            arr_material_data->m_color() = g_cfg->visuals.chams.m_local.m_color;
            g_interfaces->m_mat_sys->set_color(arr_material_data, g_cfg->visuals.chams.m_local.m_color);

            original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);

            arr_material_data->m_material() = customMaterialLocal.m_material;
            arr_material_data->m_color() = g_cfg->visuals.chams.m_local.m_color;
            g_interfaces->m_mat_sys->set_color(arr_material_data, g_cfg->visuals.chams.m_local.m_color);

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

            g_interfaces->m_mat_sys->set_color(arr_material_data, g_cfg->visuals.chams.m_weapon.m_color);

        }
    }
    else if (g_cfg->visuals.chams.m_weapon.m_enabled && pEntity->is_view_model())
    {
        auto pOwnerEntity = g_interfaces->m_entity_system->get_base_entity(pEntity->get_handle().get_entry_index());
        if (pOwnerEntity && pOwnerEntity == g_ctx->m_local_pawn)
        {
            arr_material_data->m_material() = customMaterialWeap.m_material;
            arr_material_data->m_color() = g_cfg->visuals.chams.m_weapon.m_color;
            g_interfaces->m_mat_sys->set_color(arr_material_data, g_cfg->visuals.chams.m_weapon.m_color);

        }
    }

    original(animatable_object, dx11, arr_material_data, data_count, scene_view, scene_layer, unk1, unk2);

    return true;
}