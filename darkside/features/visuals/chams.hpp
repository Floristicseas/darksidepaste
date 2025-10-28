#pragma once
#include <memory>

#include "../../darkside.hpp"

class material2_t;


enum ChamsType {
	FLAT,
	ILLUMINATE,
	GLOW,
	MAXCOUNT
};

enum ChamsEntity : std::int32_t {
	INVALID = 0,
	ENEMY,
	TEAM,
	VIEWMODEL,
	HANDS
};

enum MaterialType {
	e_visible,
	e_invisible,
	e_max_material
};

namespace chams
{
	class Materials {
	public:
		bool init();
	};

	struct resource_material_t
	{
		c_strong_handle<material2_t> mat;
		c_strong_handle<material2_t> mat_invs;
	};

	static resource_material_t resourceMaterials[ChamsType::MAXCOUNT];

	ChamsEntity GetTargetType(c_base_entity* entity) noexcept;
	c_strong_handle<material2_t> create(const char* name, const char* szVmatBuffer);

	inline material2_t* GetMaterial(int type, bool invisible) {
		return invisible ? resourceMaterials[type].mat_invs : resourceMaterials[type].mat;
	}
}
