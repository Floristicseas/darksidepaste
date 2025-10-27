#pragma once

#include "../protobufs/protobufs.hpp"

struct qucik_peek_data_t {
	void reset(vec3_t vecOrigin = vec3_t(0.f, 0.f, 0.f)) {
		m_old_origin = vecOrigin;
		m_should_retrack = false;
		m_autopeeking = false;
		m_fired = false;
		m_retracking_time = 0;
	}

	bool m_autopeeking = false;
	bool m_fired = false;
	bool m_should_retrack = false;
	int m_retracking_time = 0;
	vec3_t m_old_origin = vec3_t(0, 0, 0);
};

class c_movement {

	qucik_peek_data_t m_quick_peek_data{};

	bool m_stopped{};
	void bunnyhop(c_user_cmd* user_cmd);
	void auto_strafe();
	void directional_strafe();
	void quick_peek_assistant(c_user_cmd* user_cmd);
	void reset_quick_peek_data();
	void quick_stop(c_user_cmd* user_cmd, c_cs_player_pawn* local_pawn);
	void limit_speed(c_user_cmd* user_cmd, c_cs_player_pawn* local_player, c_base_player_weapon* active_weapon, float max_speed);

	float get_max_player_speed();
public:
	void draw_autopeek();
	void stop_movement(c_user_cmd* user_cmd, c_cs_player_pawn* local_player);
	void auto_stop(c_user_cmd* user_cmd, c_cs_player_pawn* local_player, c_base_player_weapon* active_weapon, bool no_spread);
	void movement_fix(c_user_cmd* user_cmd, vec3_t angle);
	void on_create_move(c_user_cmd* user_cmd, float old_yaw);
};

inline const auto g_movement = std::make_unique<c_movement>();