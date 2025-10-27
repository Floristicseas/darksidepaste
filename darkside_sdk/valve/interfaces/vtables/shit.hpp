#pragma once

#include "../../../darkside.hpp"

#include "utl.hpp"

struct KV3ID_t {
    SCHEMA(m_name, const char*, "KV3ID", "m_szName");
    SCHEMA(m_unk0, uint64_t, "KV3ID", "m_unk0");
    SCHEMA(m_unk1, uint64_t, "KV3ID", "m_unk1");
};

class CKeyValues3 {
public:
    SCHEMA(m_key, uint64_t, "CKeyValues3", "m_uKey");
    SCHEMA(m_value, void*, "CKeyValues3", "m_pValue");

    void LoadFromBuffer(const char* szString) {
        const int bufferSize = strlen(szString) + 10;

        CUtlBuffer buffer(0, bufferSize, 1);

        buffer.PutsString(szString);

        LoadKV3(&buffer);
    }

    bool LoadKV3(CUtlBuffer* buffer) {
        using fn_load_kv3 = bool(__thiscall*)(void*, CUtlBuffer*);
        static auto load_kv3_fn = reinterpret_cast<fn_load_kv3>(
            g_opcodes->scan_absolute("tier0.dll", "E8 ? ? ? ? EB 58 F7 43 ? ? ? ? ? 75 04 33 C9"));

        if (load_kv3_fn = nullptr)
            printf("shiit sdk");

        return load_kv3_fn(this, buffer);
    }

    static CKeyValues3* CreateMaterialResource() {
        using fn_create_material_resource = CKeyValues3 * (__fastcall*)(CKeyValues3*, unsigned int, unsigned int);
        static auto create_material_resource_fn = reinterpret_cast<fn_create_material_resource>(
            g_opcodes->scan("client.dll", "40 53 48 83 EC 30 4C 8B 11 41 B9 ? ? ? ? 49"));

        CKeyValues3* key_value = new CKeyValues3[0x10];
        return create_material_resource_fn(key_value, 1U, 6U);
    }

    static CKeyValues3* Create() {
        return CreateMaterialResource();
    }
};

inline CKeyValues3* g_key_values_system = nullptr;