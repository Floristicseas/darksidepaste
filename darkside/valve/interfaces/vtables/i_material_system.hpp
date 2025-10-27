#pragma once

#include "../../../darkside.hpp"

class material2_t {
public:
    SCHEMA(m_name, const char*, "IMaterial2", "m_name");

    virtual const char* get_name() = 0;
    virtual const char* get_shader_name() = 0;
};

struct material_key_var_t {
    SCHEMA(m_key, uint64_t, "MaterialKeyVar", "m_key");
    SCHEMA(m_name, const char*, "MaterialKeyVar", "m_name");

    material_key_var_t(uint64_t key, const char* name) {
        m_key() = key;
        m_name() = name;
    }

    material_key_var_t(const char* name, bool should_find_key = false) {
        m_name() = name;
        m_key() = should_find_key ? find_key(name) : 0x0;
    }

    std::uint64_t find_key(const char* name) {
        using function_find_key = std::uint64_t(__fastcall*)(const char*, unsigned int, int);
        static auto find_key_var = reinterpret_cast<function_find_key>(
            g_opcodes->scan("particles.dll", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 33 C0 8B DA"));

        return find_key_var(name, 0x12, 0x31415926);
    }
};

class object_info_t {
public:
    SCHEMA(m_id, int, "ObjectInfo", "m_id");
};

class scene_animable_object_t {
public:
    SCHEMA(m_owner, c_base_handle, "SceneAnimableObject", "m_hOwner");
};

class material_data_t {
public:
    SCHEMA(m_scene_animable, scene_animable_object_t*, "MaterialData", "m_pSceneAnimable");
    SCHEMA(m_material, material2_t*, "MaterialData", "m_pMaterial");
    SCHEMA(m_color, c_color, "MaterialData", "m_color");

    void set_shader_type(const char* shader_name) {
        using fn_set_material_shader_type = void(__thiscall*)(void*, material_key_var_t, const char*, int);
        static auto set_material_shader_type = reinterpret_cast<fn_set_material_shader_type>(
            g_opcodes->scan("particles.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 0F B6 01 45 0F B6 F9 8B 2A 4D 8B E0 4C 8B 72 ? 48 8B F9 C0 E8 ? 24 ? 3C ? 74 ? 41 B0 ? B2 ? E8 ? ? ? ? 0F B6 07 33 DB C0 E8 ? 24 ? 3C ? 75 ? 48 8B 77 ? EB ? 48 8B F3 4C 8D 44 24 ? C7 44 24 ? ? ? ? ? 48 8D 54 24 ? 89 6C 24 ? 48 8B CE 4C 89 74 24 ? E8 ? ? ? ? 8B D0 83 F8 ? 75 ? 45 33 C9 89 6C 24 ? 4C 8D 44 24 ? 4C 89 74 24 ? 48 8B D7 48 8B CE E8 ? ? ? ? 8B D0 0F B6 0F C0 E9 ? 80 E1 ? 80 F9 ? 75 ? 48 8B 4F ? EB ? 48 8B CB 8B 41 ? 85 C0 74 ? 48 8D 59 ? 83 F8 ? 76 ? 48 8B 1B 48 63 C2 4D 85 E4")); //fix

        material_key_var_t shader_var(0x162C1777, "shader");
        set_material_shader_type(this, shader_var, shader_name, 0x18);
    }

    void set_material_function(const char* function_name, int value) {
        using fn_set_material_function = void(__thiscall*)(void*, material_key_var_t, int, int);
        static auto set_material_function = reinterpret_cast<fn_set_material_function>(
            g_opcodes->scan("particles.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC 50 0F B6 01 45 33 E4 8B 32")); //fix

        material_key_var_t function_var(function_name, true);
        set_material_function(this, function_var, value, 0x18);
    }
};

class i_material_system {
public:
    material2_t*** find_or_create_from_resource(material2_t*** out_material, const char* material_name) {
        return vmt::call_virtual<material2_t***>(this, 14, out_material, material_name);
    }

    material2_t*** create_material(material2_t*** out_material, const char* material_name, material_data_t* data) {
        return vmt::call_virtual<material2_t***>(this, 29, out_material, material_name, data, 0, 0, 0, 0, 0, 1);
    }

    void set_create_data_by_material(const void* data, material2_t*** in_material) {
        return vmt::call_virtual<void>(this, 38, in_material, data);
    }

    static void set_color(void* data, c_color color) {
        *reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(data) + 0x50) = color.r;
        *reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(data) + 0x51) = color.g;
        *reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(data) + 0x52) = color.b;
        *reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(data) + 0x53) = color.a;
    }

    material2_t* find_material(const char* material_name) {
        material2_t** out_material = nullptr;
        if (find_or_create_from_resource(&out_material, material_name) && out_material) {
            return *out_material;
        }
        return nullptr;
    }

    material2_t* create_material(const char* material_name, material_data_t* data) {
        material2_t** out_material = nullptr;
        if (create_material(&out_material, material_name, data) && out_material) {
            return *out_material;
        }
        return nullptr;
    }
};

inline i_material_system* g_material_system = nullptr;