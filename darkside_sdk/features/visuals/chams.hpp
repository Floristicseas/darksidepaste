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
        material2_t* CreateMaterial(const char* material_name, bool ignore_z, const char* shader_name = "csgo_complex.vfx");

    private:
        bool m_initialized = false;
    };

    inline const auto chams = std::make_unique<chams_t>();
}