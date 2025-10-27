#pragma once
#include <memory>

#include "../../darkside.hpp"

using VisualChamMaterial_t = int;
enum EVisualsChamMaterials : VisualChamMaterial_t
{
    VISUAL_MATERIAL_SOLID = 0,
    VISUAL_MATERIAL_FLAT,
    VISUAL_MATERIAL_GLOW,
    VISUAL_MATERIAL_LATEX,
    VISUAL_MATERIAL_STAR,
    VISUAL_MATERIAL_ELECTRIC,
    VISUAL_MATERIAL_MAX
};

static constexpr char szVMatBufferSolidVisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    Shader = "csgo_complex.vfx"
    F_IGNOREZ = 0
    F_DISABLE_Z_WRITE = 0
    F_DISABLE_Z_BUFFERING = 0
    F_RENDER_BACKFACES = 0
    F_TRANSLUCENT = 1
    F_PAINT_VERTEX_COLORS = 1
    g_vColorTint = [1.000000, 1.000000, 1.000000, 1.000000]
    TextureNormal = resource:"materials/default/default_normal.tga"
    g_tAmbientOcclusion = resource:"materials/default/default_ao_tga_559f1ac6.vtex"
    g_tColor = resource:"materials/default/default_color_tga_72dcfbfd.vtex"
    g_tNormal = resource:"materials/default/default_normal_tga_7be61377.vtex"
})";

static constexpr char szVMatBufferSolidInvisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    Shader = "csgo_complex.vfx"
    F_IGNOREZ = 1
    F_DISABLE_Z_WRITE = 1
    F_DISABLE_Z_BUFFERING = 1
    F_RENDER_BACKFACES = 0
    F_TRANSLUCENT = 1
    F_PAINT_VERTEX_COLORS = 1
    g_vColorTint = [1.000000, 1.000000, 1.000000, 1.000000]
    TextureNormal = resource:"materials/default/default_normal.tga"
    g_tAmbientOcclusion = resource:"materials/default/default_ao_tga_559f1ac6.vtex"
    g_tColor = resource:"materials/default/default_color_tga_72dcfbfd.vtex"
    g_tNormal = resource:"materials/default/default_normal_tga_7be61377.vtex"
})";

static constexpr char szVMatBufferWhiteVisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    shader = "csgo_unlitgeneric.vfx"
    F_PAINT_VERTEX_COLORS = 1
    F_TRANSLUCENT = 1
    F_BLEND_MODE = 1
    g_vColorTint = [1, 1, 1, 1]
    TextureAmbientOcclusion = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tAmbientOcclusion = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tNormal = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tTintMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
})";

static constexpr char szVMatBufferWhiteInvisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    shader = "csgo_unlitgeneric.vfx"
    F_PAINT_VERTEX_COLORS = 1
    F_TRANSLUCENT = 1
    F_BLEND_MODE = 1
    F_DISABLE_Z_BUFFERING = 1
    g_vColorTint = [1, 1, 1, 1]
    TextureAmbientOcclusion = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tAmbientOcclusion = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tNormal = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tTintMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
})";

static constexpr char szVMatBufferIlluminateVisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    shader = "solidcolor.vfx"
    F_SELF_ILLUM = 1
    F_PAINT_VERTEX_COLORS = 1
    F_TRANSLUCENT = 1 
    F_IGNOREZ = 0
    F_DISABLE_Z_WRITE = 0
    F_DISABLE_Z_BUFFERING = 0
    g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tNormal = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
    g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
    g_tSelfIllumMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    TextureAmbientOcclusion = resource:"materials/debug/particleerror.vtex"
    g_tAmbientOcclusion = resource:"materials/debug/particleerror.vtex"
    g_vColorTint = [ 20.000000, 20.000000, 20.000000, 20.000000 ]
    g_flSelfIllumScale = [ 5.000000, 5.000000, 5.000000, 5.000000 ]
    g_flSelfIllumBrightness = [ 5.000000, 5.000000, 5.000000, 5.000000 ]
    g_vSelfIllumTint = [ 10.000000, 10.000000, 10.000000, 10.000000 ]
})";

static constexpr char szVMatBufferIlluminateInvisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    shader = "solidcolor.vfx"
    F_SELF_ILLUM = 1
    F_PAINT_VERTEX_COLORS = 1
    F_TRANSLUCENT = 1 
    F_IGNOREZ = 1
    F_DISABLE_Z_WRITE = 1
    F_DISABLE_Z_BUFFERING = 1
    g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tNormal = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
    g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
    g_tSelfIllumMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    TextureAmbientOcclusion = resource:"materials/debug/particleerror.vtex"
    g_tAmbientOcclusion = resource:"materials/debug/particleerror.vtex"
    g_vColorTint = [ 20.000000, 20.000000, 20.000000, 20.000000 ]
    g_flSelfIllumScale = [ 5.000000, 5.000000, 5.000000, 5.000000 ]
    g_flSelfIllumBrightness = [ 5.000000, 5.000000, 5.000000, 5.000000 ]
    g_vSelfIllumTint = [ 10.000000, 10.000000, 10.000000, 10.000000 ]
})";

static constexpr char szVMatBufferLatexVisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
				Shader = "csgo_effects.vfx"
                g_flFresnelExponent = 7.0
                g_flFresnelFalloff = 10.0
                g_flFresnelMax = 0.1
                g_flFresnelMin = 1.0
				g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tMask1 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tMask2 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tMask3 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tSceneDepth = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_flToolsVisCubemapReflectionRoughness = 1.0
                g_flBeginMixingRoughness = 1.0
                g_vColorTint = [ 1.000000, 1.000000, 1.000000, 0 ]
                F_IGNOREZ = 0
                F_DISABLE_Z_WRITE = 0
                F_DISABLE_Z_BUFFERING = 0
                F_RENDER_BACKFACES = 0
})";

static constexpr char szVMatBufferLatexInvisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
				Shader = "csgo_effects.vfx"
                g_flFresnelExponent = 7.0
                g_flFresnelFalloff = 10.0
                g_flFresnelMax = 0.1
                g_flFresnelMin = 1.0
				g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tMask1 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tMask2 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tMask3 = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_tSceneDepth = resource:"materials/default/default_mask_tga_fde710a5.vtex"
                g_flToolsVisCubemapReflectionRoughness = 1.0
                g_flBeginMixingRoughness = 1.0
                g_vColorTint = [ 1.000000, 1.000000, 1.000000, 0 ]
                F_IGNOREZ = 1
                F_DISABLE_Z_WRITE = 1
                F_DISABLE_Z_BUFFERING = 1
                F_RENDER_BACKFACES = 0
})";

static constexpr char szVMatBufferStarVisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    shader = "csgo_effects.vfx"
    F_ADDITIVE_BLEND = 1
    F_BLEND_MODE = 1
    F_TRANSLUCENT = 1
    g_vColorTint = [7.0, 7.0, 7.0, 0.37522]
    g_flColorBoost = 30
    g_flOpacityScale = 245.55
    g_flFresnelExponent = 7.75
    g_flFresnelFalloff = 5
    g_flFresnelMax = 0.0
    g_flFresnelMin = 9
    g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
    g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
    g_tMask1 = resource:"materials/default/default_mask_tga_344101f8.vtex"
    g_tMask2 = resource:"materials/default/default_mask_tga_344101f8.vtex"
    TextureAmbientOcclusion = resource:"materials/default/default_ao_tga_559f1ac6.vtex"
    g_tMask3 = resource:"materials/default/default_mask_tga_344101f8.vtex"
    g_tSelfIllumMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
    g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
    g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
})";

static constexpr char szVMatBufferStarInvisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    shader = "csgo_effects.vfx"
    F_ADDITIVE_BLEND = 1
    F_BLEND_MODE = 1
    F_TRANSLUCENT = 1
    F_DISABLE_Z_BUFFERING = 1
    g_vColorTint = [7.0, 7.0, 7.0, 0.37522]
    g_flColorBoost = 30
    g_flOpacityScale = 245.55
    g_flFresnelExponent = 7.75
    g_flFresnelFalloff = 5
    g_flFresnelMax = 0.0
    g_flFresnelMin = 9
    g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
    g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
    g_tMask1 = resource:"materials/default/default_mask_tga_344101f8.vtex"
    g_tMask2 = resource:"materials/default/default_mask_tga_344101f8.vtex"
    TextureAmbientOcclusion = resource:"materials/default/default_ao_tga_559f1ac6.vtex"
    g_tMask3 = resource:"materials/default/default_mask_tga_344101f8.vtex"
    g_tSelfIllumMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
    g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
    g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
})";

static constexpr char szVMatBufferElectricVisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    shader = "csgo_complex.vfx"
    F_SELF_ILLUM = 1
    F_SELF_ILLUM_FRESNEL = 1
    F_PAINT_VERTEX_COLORS = 1
    F_TRANSLUCENT = 1
    F_ADDITIVE_BLEND = 1
    g_vColorTint = [ 0.200000, 0.600000, 1.000000, 0.800000 ]
    g_flSelfIllumScale = [ 2.000000, 2.000000, 2.000000, 2.000000 ]
    g_flSelfIllumBrightness = [ 3.000000, 3.000000, 3.000000, 3.000000 ]
    g_vSelfIllumTint = [ 0.300000, 0.700000, 1.000000, 1.000000 ]
    g_flSelfIllumFresnelScale = 2.500000
    g_flSelfIllumFresnelBias = 0.100000
    g_flElectricSpeed = 4.000000
    g_flElectricScale = 0.800000
    g_flElectricIntensity = 1.200000
    g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tNormal = resource:"materials/default/default_normal_tga_7be61377.vtex"
    g_tSelfIllumMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    TextureAmbientOcclusion = resource:"materials/default/default_ao_tga_559f1ac6.vtex"
    g_tAmbientOcclusion = resource:"materials/default/default_ao_tga_559f1ac6.vtex"
    g_tElectricTexture = resource:"materials/default/default_fresnelwarp_tga_d9279d65.vtex"
})";

static constexpr char szVMatBufferElectricInvisible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
{
    shader = "csgo_complex.vfx"
    F_SELF_ILLUM = 1
    F_SELF_ILLUM_FRESNEL = 1
    F_PAINT_VERTEX_COLORS = 1
    F_TRANSLUCENT = 1
    F_ADDITIVE_BLEND = 1
    F_DISABLE_Z_BUFFERING = 1
    g_vColorTint = [ 0.200000, 0.600000, 1.000000, 0.600000 ]
    g_flSelfIllumScale = [ 2.000000, 2.000000, 2.000000, 2.000000 ]
    g_flSelfIllumBrightness = [ 3.000000, 3.000000, 3.000000, 3.000000 ]
    g_vSelfIllumTint = [ 0.300000, 0.700000, 1.000000, 1.000000 ]
    g_flSelfIllumFresnelScale = 2.500000
    g_flSelfIllumFresnelBias = 0.100000
    g_flElectricSpeed = 4.000000
    g_flElectricScale = 0.800000
    g_flElectricIntensity = 1.200000
    g_tColor = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    g_tNormal = resource:"materials/default/default_normal_tga_7be61377.vtex"
    g_tSelfIllumMask = resource:"materials/default/default_mask_tga_fde710a5.vtex"
    TextureAmbientOcclusion = resource:"materials/default/default_ao_tga_559f1ac6.vtex"
    g_tAmbientOcclusion = resource:"materials/default/default_ao_tga_559f1ac6.vtex"
    g_tElectricTexture = resource:"materials/default/default_fresnelwarp_tga_d9279d65.vtex"
})";

class material_data_t;
class material2_t;

namespace chams
{
    class chams_t {
    public:
        bool initialize();
        bool draw_object(void* animatable_object, void* dx11, material_data_t* arr_material_data, int data_count,
            void* scene_view, void* scene_layer, void* unk1, void* unk2);
        bool override_material(void* animatable_object, void* dx11, material_data_t* arr_material_data, int data_count,
            void* scene_view, void* scene_layer, void* unk1, void* unk2);

        material2_t* CreateMaterial(const char* szMaterialName, const char szVmatBuffer[]);

    private:
        bool m_initialized = false;
    };

    inline const auto chams = std::make_unique<chams_t>();
}