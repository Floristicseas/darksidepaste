#include "../../darkside.hpp"

class CGameEvent;

struct OnShotSkeleton_t
{
	c_cs_player_pawn* m_pPawn;
	c_cs_player_controller* m_pController;
	c_bone_data m_Matrix[128];
	float m_flTimeAdded;
};

class CShotHandler {
public:
	struct PlayerHurtRecord_t {
		int m_iHitGroup;
		vec3_t m_vecBoneOrigin;
		vec3_t m_vecImpactPosition;
		float m_flTime;
	};
public:
	std::deque<PlayerHurtRecord_t> m_HitRecords;
public:
	void RenderHitMarker(const ImVec2& pos, float size = 6.0f);
	void OnPlayerHurt(CGameEvent* pEvent);
	void OnBulletImpact(CGameEvent* pEvent);
	void OnPresent();
};

inline auto g_ShotHandler = std::make_unique<CShotHandler>();
