#include "movement.hpp"
#include "../eng_pred/eng_pred.hpp"
#include "../../render/render.hpp"
#include "../../menu/custom/keybinds.hpp"

constexpr float M_PI = 3.14159265358979323846f;
constexpr float M_PI_F = static_cast<float>(M_PI);
constexpr float RAD2DEG = 180.f / M_PI_F;
constexpr float DEG2RAD = M_PI_F / 180.f;

void c_movement::bunnyhop(c_user_cmd* user_cmd)
{
	if (!g_cfg->misc.m_hop)
		return;

	if (g_ctx->m_local_pawn->m_flags() & FL_ONGROUND)
		user_cmd->m_button_state.m_button_state &= ~IN_JUMP;
}

__forceinline float normalize(float value)
{
	while (value > 180.f)
		value -= 360.f;

	while (value < -180.f)
		value += 360.f;

	return value;
}

void c_movement::directional_strafe()
{
	if (!g_interfaces->m_engine->is_connected() || !g_interfaces->m_engine->is_in_game())
		return;

	if (!g_ctx->m_local_pawn || !g_ctx->m_local_controller->m_pawn_is_alive())
		return;

	if (g_ctx->m_local_pawn->m_flags() & FL_ONGROUND)
		return;

	if (g_ctx->m_local_pawn->m_move_type() == 1794 || g_ctx->m_local_pawn->m_move_type() == 2313)
		return;

	if (!g_cfg->misc.m_auto_strafe)
		return;

	if (g_ctx->m_user_cmd->m_button_state.m_button_state & (1 << 16))
		return;

	auto movement_services = g_ctx->m_local_pawn->m_movement_services();
	if (!movement_services)
		return;

	vec3_t velocity = g_ctx->m_local_pawn->m_vec_abs_velocity();
	float speed = velocity.length_2d();

	if (speed < 5.0f)
		return;

	bool pressing_left = g_ctx->m_user_cmd->m_button_state.m_button_state & IN_MOVELEFT;
	bool pressing_right = g_ctx->m_user_cmd->m_button_state.m_button_state & IN_MOVERIGHT;
	bool pressing_forward = g_ctx->m_user_cmd->m_button_state.m_button_state & IN_FORWARD;
	bool pressing_back = g_ctx->m_user_cmd->m_button_state.m_button_state & IN_BACK;

	static bool was_pressing_left = false;
	static bool was_pressing_right = false;
	static float last_direction = 0.f;

	if (pressing_left && !was_pressing_left) {
		last_direction = -1.f; 
	}
	if (pressing_right && !was_pressing_right) {
		last_direction = 1.f; 
	}

	was_pressing_left = pressing_left;
	was_pressing_right = pressing_right;

	if (last_direction == 0.f || (!pressing_left && !pressing_right))
		return;

	float wish_yaw = last_direction * 90.f;

	if (pressing_forward)
		wish_yaw *= 0.5f;
	else if (pressing_back)
		wish_yaw = -wish_yaw * 0.5f + 180.f;

	float velocity_dir = RAD2DEG * std::atan2(velocity.y, velocity.x);
	velocity_dir = normalize(velocity_dir);

	float optimal_angle = 0.f;
	if (g_cfg->misc.m_directional_type == 0) { // Default
		optimal_angle = std::clamp(RAD2DEG * std::atan(15.f / speed), 0.f, 45.f);
	}
	else { // Rage
		optimal_angle = std::clamp(RAD2DEG * std::atan(30.f / speed), 0.f, 90.f);
	}

	float smooth_factor = g_cfg->misc.m_strafe_smooth / 100.f;
	optimal_angle *= (1.0f - smooth_factor * 0.5f);

	float target_yaw = velocity_dir + (wish_yaw > 0 ? optimal_angle : -optimal_angle);
	target_yaw = normalize(target_yaw);

	if (g_cfg->misc.m_silent_strafe) {
		vec3_t move_angles;
		g_math->vector_angles(velocity, move_angles);

		float angle_diff = normalize(target_yaw - move_angles.y);
		float move_x = std::cos(DEG2RAD * angle_diff);
		float move_y = std::sin(DEG2RAD * angle_diff);

		g_ctx->m_user_cmd->pb.mutable_base()->set_forwardmove(move_x * 450.f);
		g_ctx->m_user_cmd->pb.mutable_base()->set_leftmove(move_y * -450.f);
	}
	else {
		g_ctx->m_user_cmd->pb.mutable_base()->mutable_viewangles()->set_y(target_yaw);
		g_ctx->m_user_cmd->pb.mutable_base()->set_forwardmove(0.f);
		g_ctx->m_user_cmd->pb.mutable_base()->set_leftmove(wish_yaw > 0 ? -1.f : 1.f);
	}
}

void c_movement::auto_strafe()
{
	if (!g_cfg->misc.m_auto_strafe)
		return;
	
	if (g_cfg->misc.m_auto_strafe_type == 0) { 
		directional_strafe();
		return;
	}

	// TODO: add silent strafe
	if (!g_interfaces->m_engine->is_connected() || !g_interfaces->m_engine->is_in_game())
		return;

	if (!g_ctx->m_local_pawn || !g_ctx->m_local_controller->m_pawn_is_alive())
		return;

	if (g_ctx->m_local_pawn->m_flags() & FL_ONGROUND)
		return;

	if (g_ctx->m_local_pawn->m_move_type() == 1794 || g_ctx->m_local_pawn->m_move_type() == 2313)
		return;

	if (g_ctx->m_user_cmd->m_button_state.m_button_state & (1 << 16))
		return;

	auto movement_services = g_ctx->m_local_pawn->m_movement_services();
	if (!movement_services)
		return;

	vec3_t vel = g_ctx->m_local_pawn->m_vec_abs_velocity();
	vec3_t move = movement_services->m_last_movement_impulses();
	float yaw = normalize(g_interfaces->m_csgo_input->get_view_angles().y);

	static uint64_t last_pressed = 0;
	static uint64_t last_buttons = 0;
	const auto current_buttons = g_ctx->m_user_cmd->m_button_state.m_button_state;

	auto check_button = [&](const uint64_t button) {
		if (current_buttons & button && (!(last_buttons & button) ||
			(button & IN_MOVELEFT && !(last_pressed & IN_MOVERIGHT)) ||
			(button & IN_MOVERIGHT && !(last_pressed & IN_MOVELEFT)) ||
			(button & IN_FORWARD && !(last_pressed & IN_BACK)) ||
			(button & IN_BACK && !(last_pressed & IN_FORWARD)))) {
			if (button & IN_MOVELEFT)
				last_pressed &= ~IN_MOVERIGHT;
			else if (button & IN_MOVERIGHT)
				last_pressed &= ~IN_MOVELEFT;
			else if (button & IN_FORWARD)
				last_pressed &= ~IN_BACK;
			else if (button & IN_BACK)
				last_pressed &= ~IN_FORWARD;

			last_pressed |= button;
		}
		else if (!(current_buttons & button))
			last_pressed &= ~button;
		};

	check_button(IN_MOVELEFT);
	check_button(IN_MOVERIGHT);
	check_button(IN_FORWARD);
	check_button(IN_BACK);
	last_buttons = current_buttons;

	float offset = 0.f;
	if (last_pressed & IN_MOVELEFT)
		offset += 90.f;
	if (last_pressed & IN_MOVERIGHT)
		offset -= 90.f;
	if (last_pressed & IN_FORWARD)
		offset *= 0.5f;
	else if (last_pressed & IN_BACK)
		offset = -offset * 0.5f + 180.f;

	yaw += offset;

	float velocity_angle = RAD2DEG * std::atan2f(vel.y, vel.x);
	if (velocity_angle < 0.0f)
		velocity_angle += 360.0f;
	velocity_angle -= floorf(velocity_angle / 360.0f + 0.5f) * 360.0f;

	float speed = vel.length_2d();
	float ideal = speed > 0.f ? std::clamp(RAD2DEG * std::atan2(40.f / speed, 1.f), 0.0f, 90.0f) : 90.0f;
	float correct = (100.f - 100) * 0.01f * ideal;

	auto rotate_movement = [](float target_yaw) {
		float rot = DEG2RAD * (g_interfaces->m_csgo_input->get_view_angles().y - target_yaw);
		float new_forward = std::cos(rot) * g_ctx->m_user_cmd->pb.mutable_base()->forwardmove() -
			std::sin(rot) * g_ctx->m_user_cmd->pb.mutable_base()->leftmove();
		float new_side = std::sin(rot) * g_ctx->m_user_cmd->pb.mutable_base()->forwardmove() +
			std::cos(rot) * g_ctx->m_user_cmd->pb.mutable_base()->leftmove();

		g_ctx->m_user_cmd->m_button_state.m_button_state &= ~(IN_BACK | IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT);
		g_ctx->m_user_cmd->m_button_state.m_button_state2 &= ~(IN_BACK | IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT);

		g_ctx->m_user_cmd->pb.mutable_base()->set_forwardmove(std::clamp(new_forward, -1.f, 1.f));
		g_ctx->m_user_cmd->pb.mutable_base()->set_leftmove(std::clamp(new_side * -1.f, -1.f, 1.f));

		if (g_ctx->m_user_cmd->pb.mutable_base()->forwardmove() > 0.0f)
			g_ctx->m_user_cmd->m_button_state.m_button_state |= IN_FORWARD;
		else if (g_ctx->m_user_cmd->pb.mutable_base()->forwardmove() < 0.0f)
			g_ctx->m_user_cmd->m_button_state.m_button_state |= IN_BACK;
		if (g_ctx->m_user_cmd->pb.mutable_base()->leftmove() > 0.0f)
			g_ctx->m_user_cmd->m_button_state.m_button_state |= IN_MOVELEFT;
		else if (g_ctx->m_user_cmd->pb.mutable_base()->leftmove() < 0.0f)
			g_ctx->m_user_cmd->m_button_state.m_button_state |= IN_MOVERIGHT;
		};

	float velocity_delta = normalize(yaw - velocity_angle);
	if (fabsf(velocity_delta) > 90.f)
		velocity_delta = std::copysignf(90.f, velocity_delta);

	g_ctx->m_user_cmd->pb.mutable_base()->set_forwardmove(0.f);
	g_ctx->m_user_cmd->pb.mutable_base()->set_leftmove(0.f);

	if (speed <= 80.f) {
		yaw += ideal * 3.0f;
		g_ctx->m_user_cmd->pb.mutable_base()->set_leftmove(1.0f);
	}
	else if (velocity_delta > correct) {
		yaw = velocity_angle + correct * 4.0f;
		g_ctx->m_user_cmd->pb.mutable_base()->set_leftmove(-1.0f);
	}
	else if (velocity_delta < -correct) {
		yaw = velocity_angle - correct * 4.0f;
		g_ctx->m_user_cmd->pb.mutable_base()->set_leftmove(1.0f);
	}
	else {
		yaw += ideal * 4.0f;
		g_ctx->m_user_cmd->pb.mutable_base()->set_leftmove(1.0f);
	}

	rotate_movement(normalize(yaw));
}

void c_movement::quick_peek_assistant(c_user_cmd* user_cmd)
{
	static vec3_t vec_origin = vec3_t(0.0f, 0.0f, 0.0f);

	auto base_cmd = user_cmd->pb.mutable_base();

	bool processing_movement_keys = (user_cmd->m_button_state.m_button_state & (IN_MOVELEFT) || user_cmd->m_button_state.m_button_state & (IN_FORWARD) || user_cmd->m_button_state.m_button_state & (IN_BACK) ||
		user_cmd->m_button_state.m_button_state & (IN_MOVERIGHT) || user_cmd->m_button_state.m_button_state & (IN_MOVELEFT) || user_cmd->m_button_state.m_button_state & (IN_MOVERIGHT) ||
		user_cmd->m_button_state.m_button_state & (IN_JUMP));

	if (!g_key_handler->is_pressed(g_cfg->misc.m_override_quick_peek_assistant, g_cfg->misc.m_override_quick_peek_assistant_style))
	{
		if (g_cfg->misc.m_auto_peek_return) {
			m_quick_peek_data.m_should_retrack = true;
		}
		else {
			m_quick_peek_data.reset();
		}
		return;
	}

	if (m_quick_peek_data.m_old_origin.is_zero()) {
		vec_origin = g_ctx->m_local_pawn->m_scene_node()->m_origin();

		game_trace_t trace;
		trace_filter_t filter(0x3001, nullptr, nullptr, 3);
		filter.m_arr_collisions.at(1) = 0x10000;

		ray_t ray = ray_t();
		g_interfaces->m_trace->trace_shape(&ray, g_prediction->get_local_data()->m_eye_pos, g_prediction->get_local_data()->m_eye_pos - vec3_t(0.0f, 0.0f, 1000.f), &filter, &trace);

		if (trace.m_fraction < 1.0f)
			vec_origin = trace.m_end + vec3_t(0.0f, 0.0f, 2.0f);

		m_quick_peek_data.reset(vec_origin);
	}
	else
	{
		if (g_ctx->m_local_pawn->m_scene_node()->m_origin().dist(m_quick_peek_data.m_old_origin) > 5.0f)
		{
			if (m_quick_peek_data.m_should_retrack)
			{
				++m_quick_peek_data.m_retracking_time;

				vec3_t ang_angles_to_pos = vec3_t(0, 0, 0);
				g_math->vector_angles(m_quick_peek_data.m_old_origin - g_ctx->m_local_pawn->m_scene_node()->m_origin(), ang_angles_to_pos);

				base_cmd->set_forwardmove(std::fmaxf(std::fminf(1.0f, static_cast<float>(std::cos(deg2rad((base_cmd->viewangles().y() - ang_angles_to_pos.y))) * static_cast<float>(std::cos(ang_angles_to_pos.x * 0.017453292)))), -1.0f));
				base_cmd->set_leftmove(-std::fmaxf(std::fminf(1.0f, static_cast<float>(std::sin(deg2rad((base_cmd->viewangles().y() - ang_angles_to_pos.y))) * static_cast<float>(std::cos(ang_angles_to_pos.x * 0.017453292)))), -1.0f));
			}
		}
		else
		{
			m_quick_peek_data.m_should_retrack = false;
			m_quick_peek_data.m_fired = false;
			m_quick_peek_data.m_retracking_time = 0;
		}
	}

	if (user_cmd->m_button_state.m_button_state & IN_ATTACK || !processing_movement_keys && g_cfg->misc.m_quick_peek_assistant_type == 0)
	{
		m_quick_peek_data.m_should_retrack = true;
		m_quick_peek_data.m_autopeeking = true;

		if (user_cmd->m_button_state.m_button_state & IN_ATTACK)
			m_quick_peek_data.m_fired = true;
	}
	else if (!user_cmd->m_button_state.m_button_state & IN_ATTACK && (processing_movement_keys && g_cfg->misc.m_quick_peek_assistant_type == 1) && m_quick_peek_data.m_should_retrack && !m_quick_peek_data.m_fired)
		m_quick_peek_data.m_should_retrack = false;
}

void c_movement::draw_autopeek()
{
	if (!g_key_handler->is_pressed(g_cfg->misc.m_override_quick_peek_assistant, g_cfg->misc.m_override_quick_peek_assistant_style))
		g_keybinds_display->remove("Quick Peek");

	if (!g_interfaces->m_engine->is_connected() || !g_interfaces->m_engine->is_in_game() || !g_ctx->m_local_pawn)
		return;

	vec3_t position = vec3_t();

	if (!m_quick_peek_data.m_old_origin.is_zero())
		position = m_quick_peek_data.m_old_origin;	

	if (position.is_zero())
		return;

	if (g_key_handler->is_pressed(g_cfg->misc.m_override_quick_peek_assistant, g_cfg->misc.m_override_quick_peek_assistant_style))
	{
		g_keybinds_display->push("Quick Peek", g_cfg->misc.m_override_quick_peek_assistant_style);

		vec3_t circle_pos = position;
		circle_pos.z += 3.0f; 

		vec3_t screen_pos;
		//g_render->circle_3d(circle_pos, 30.0f, c_color(255, 255, 255, 255), 32);

		static float m_last_time = 0.f;
		float m_current_time = g_interfaces->m_global_vars->m_current_time;

		if (m_current_time - m_last_time < 1.5f)
			return;

		m_last_time = m_current_time;

	}	
}

void c_movement::quick_stop(c_user_cmd* user_cmd, c_cs_player_pawn* local_pawn)
{
	if (!g_cfg->misc.m_quick_stop)
		return;

	if (m_quick_peek_data.m_should_retrack && m_quick_peek_data.m_retracking_time > 0)
		return;

	if (!local_pawn || !local_pawn->is_alive())
		return;

	if (local_pawn->m_move_type() == movetype_noclip || local_pawn->m_move_type() == movetype_ladder || !(local_pawn->m_flags() & FL_ONGROUND) || (user_cmd->m_button_state.m_button_state & IN_JUMP) || (user_cmd->m_button_state.m_button_state & (IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK)))
		return;

	vec3_t velocity = local_pawn->m_vec_abs_velocity();
	float speed = velocity.length_2d();
	if (speed <= 15.f)
		return;

	vec3_t view_angles = g_interfaces->m_csgo_input->get_view_angles();
	view_angles.x = 0.f;

	float sy, cy;
	g_math->sin_cos(DEG2RAD * view_angles.y, sy, cy);

	vec3_t local_velocity;
	local_velocity.x = velocity.x * cy + velocity.y * sy;
	local_velocity.y = -velocity.x * sy + velocity.y * cy;
	local_velocity.z = 0.f;

	float local_speed = local_velocity.length_2d();
	if (local_speed <= 0.001f)
		return;

	local_velocity /= local_speed;

	vec3_t stop_direction;
	stop_direction.x = -local_velocity.x;
	stop_direction.y = -local_velocity.y;
	stop_direction.z = -local_velocity.z;

	auto quantize = [](float value) -> float { int8_t quantized = static_cast<int8_t>(value * 127.0f); return static_cast<float>(quantized) / 127.0f; };

	auto sv_quantize_movement_input = g_interfaces->m_var->get_by_name(xorstr_("sv_quantize_movement_input"));
	if (sv_quantize_movement_input && sv_quantize_movement_input->get_int()) {
		user_cmd->pb.mutable_base()->set_forwardmove(quantize(stop_direction.x));
		user_cmd->pb.mutable_base()->set_leftmove(quantize(stop_direction.y));
	}
	else {
		user_cmd->pb.mutable_base()->set_forwardmove(stop_direction.x);
		user_cmd->pb.mutable_base()->set_leftmove(stop_direction.y);
	}

	user_cmd->m_button_state.m_button_state |= IN_FORWARD;
	user_cmd->m_button_state.m_button_state |= IN_MOVELEFT;
}

void c_movement::stop_movement(c_user_cmd* user_cmd, c_cs_player_pawn* local_player)
{
	if (!local_player)
		return;

	vec3_t velocity = local_player->m_vec_velocity();
	float speed = velocity.length_2d();

	if (speed < 1.f)
	{
		user_cmd->pb.mutable_base()->set_forwardmove(0.f);
		user_cmd->pb.mutable_base()->set_leftmove(0.f);
		return;
	}

	vec3_t velocity_angle;
	g_math->vector_angles(velocity * -1.f, velocity_angle);
	velocity_angle.y = g_interfaces->m_csgo_input->get_view_angles().y - velocity_angle.y;

	vec3_t forward;
	g_math->angle_vectors(velocity_angle, forward);

	float stop_power = std::min<float>(speed * 2.0f, get_max_player_speed());

	user_cmd->pb.mutable_base()->set_forwardmove(forward.x * stop_power);
	user_cmd->pb.mutable_base()->set_leftmove(forward.y * -stop_power);

	if (speed > 50.f)
	{
		auto stop_step = g_protobuf->add_subtick_move_step(user_cmd);
		stop_step->set_button(IN_SPEED);
		stop_step->set_when(0.5f);
		stop_step->set_analog_forward_delta(-user_cmd->pb.mutable_base()->forwardmove());
		stop_step->set_analog_left_delta(-user_cmd->pb.mutable_base()->leftmove());
	}
}

void c_movement::reset_quick_peek_data()
{
	if (!g_key_handler->is_pressed(g_cfg->misc.m_override_quick_peek_assistant, g_cfg->misc.m_override_quick_peek_assistant_style))
		m_quick_peek_data.reset(vec3_t(0.f, 0.f, 0.f));
}

float c_movement::get_max_player_speed()
{
	if (!g_ctx->m_local_pawn->get_active_weapon() || !g_ctx->m_local_pawn->get_active_weapon()->get_weapon_data())
		return 260.f;

	return  g_ctx->m_local_pawn->m_scoped() ? g_ctx->m_local_pawn->get_active_weapon()->get_weapon_data()->m_max_speed()[1] : g_ctx->m_local_pawn->get_active_weapon()->get_weapon_data()->m_max_speed()[0];
}

void c_movement::limit_speed( c_user_cmd* user_cmd, c_cs_player_pawn* local_player, c_base_player_weapon* active_weapon, float max_speed ) {
	c_player_movement_service* movement_services = local_player->m_movement_services( );

	if ( !movement_services )
		return;

	vec3_t velocity = g_ctx->m_local_pawn->m_vec_abs_velocity( );

	float cmd_speed = std::sqrt( 
		( user_cmd->pb.mutable_base( )->leftmove( ) * user_cmd->pb.mutable_base( )->leftmove( ) ) 
		+ ( user_cmd->pb.mutable_base( )->forwardmove( ) * user_cmd->pb.mutable_base( )->forwardmove( ) ) 
	);

	float speed_2d = velocity.length_2d( );

	if ( cmd_speed <= 50.f
		&& speed_2d <= 50.f )
		return;

	float accelerate = g_interfaces->m_var->get_by_name( xorstr_( "sv_accelerate" ) )->get_float( );

	vec3_t view_angles = {
		g_ctx->m_user_cmd->pb.mutable_base( )->viewangles( ).x( ),
		g_ctx->m_user_cmd->pb.mutable_base( )->viewangles( ).y( ),
		g_ctx->m_user_cmd->pb.mutable_base( )->viewangles( ).z( )
	};

	vec3_t forward{ }, right{ }, up{ };
	g_math->angle_vectors( view_angles, forward, right, up );

	float diff = speed_2d - max_speed;
	float wish_speed = max_speed;

	vec3_t direction = { forward.x * user_cmd->pb.mutable_base( )->forwardmove( ) + right.x * user_cmd->pb.mutable_base( )->leftmove( ),
		forward.y * user_cmd->pb.mutable_base( )->forwardmove( ) + right.y * user_cmd->pb.mutable_base( )->leftmove( ), 0.f };

	const float max_accelerate = accelerate * INTERVAL_PER_TICK * max( 250.f, movement_services->m_max_speed( ) * movement_services->m_surface_friction( ) );

	if ( diff - max_accelerate <= 0.f
		|| speed_2d - max_accelerate - 3.f <= 0.f )
		wish_speed = max_speed;
	else {
		direction = velocity;
		wish_speed = -1.f;
	}

	if ( user_cmd->pb.mutable_base( )->forwardmove( ) > 0 )
		user_cmd->pb.mutable_base( )->set_forwardmove( wish_speed );
	else if ( user_cmd->pb.mutable_base( )->forwardmove( ) < 0 )
		user_cmd->pb.mutable_base( )->set_forwardmove( -wish_speed );
	
	if ( user_cmd->pb.mutable_base( )->leftmove( ) > 0 )
		user_cmd->pb.mutable_base( )->set_leftmove( wish_speed );
	else if ( user_cmd->pb.mutable_base( )->leftmove( ) < 0 )
		user_cmd->pb.mutable_base( )->set_leftmove( -wish_speed );
}

void c_movement::auto_stop(c_user_cmd* user_cmd, c_cs_player_pawn* local_player, c_base_player_weapon* active_weapon, bool no_spread)
{
	if (!g_cfg->rage_bot.m_auto_stop)
		return;

	if (no_spread)
		return;

	if (!(local_player->m_flags() & FL_ONGROUND))
		return;

	if (local_player->m_vec_velocity().length_2d() <= 0.f)
		return;

	float max_accurate_speed = get_max_player_speed() * 0.25f;
	float wish_speed = active_weapon->get_max_speed() * 0.25f;
	if (local_player->m_vec_velocity().length_2d() <= max_accurate_speed)
		return limit_speed(user_cmd, local_player, active_weapon, wish_speed);

	stop_movement(user_cmd, local_player);
}

void c_movement::movement_fix( c_user_cmd* user_cmd, vec3_t angle )
{
	if (user_cmd->pb.mutable_base()->forwardmove() > 0.f)
		user_cmd->m_button_state.set_button_state(IN_FORWARD, c_in_button_state::e_button_state_t::in_button_down);
	else if (user_cmd->pb.mutable_base()->forwardmove() < 0.f)
		user_cmd->m_button_state.set_button_state(IN_BACK, c_in_button_state::e_button_state_t::in_button_down);

	if (user_cmd->pb.mutable_base()->leftmove() > 0.f)
		user_cmd->m_button_state.set_button_state(IN_MOVELEFT, c_in_button_state::e_button_state_t::in_button_down);
	else if (user_cmd->pb.mutable_base()->leftmove() < 0.f)
		user_cmd->m_button_state.set_button_state(IN_MOVERIGHT, c_in_button_state::e_button_state_t::in_button_down);

	vec3_t wish_angle{ user_cmd->pb.mutable_base( )->viewangles( ).x( ), user_cmd->pb.mutable_base( )->viewangles( ).y( ), user_cmd->pb.mutable_base( )->viewangles( ).z( ) };
	int sign = wish_angle.x > 89.f ? -1.f : 1.f;
	wish_angle.clamp( );

	vec3_t forward, right, up, old_forward, old_right, old_up;
	vec3_t view_angles = angle;

	g_math->angle_vectors( wish_angle, forward, right, up );

	forward.z = right.z = up.x = up.y = 0.f;

	forward.normalize_in_place( );
	right.normalize_in_place( );
	up.normalize_in_place( );

	g_math->angle_vectors( view_angles, old_forward, old_right, old_up );

	old_forward.z = old_right.z = old_up.x = old_up.y = 0.f;

	old_forward.normalize_in_place( );
	old_right.normalize_in_place( );
	old_up.normalize_in_place( );

	forward *= user_cmd->pb.mutable_base( )->forwardmove( );
	right *= user_cmd->pb.mutable_base( )->leftmove( );
	up *= user_cmd->pb.mutable_base( )->upmove( );

	float fixed_forward_move = old_forward.dot( right ) + old_forward.dot( forward ) + old_forward.dot( up, true );

	float fixed_side_move = old_right.dot( right ) + old_right.dot( forward ) + old_right.dot( up, true );

	float fixed_up_move = old_up.dot( right, true ) + old_up.dot( forward, true ) + old_up.dot( up );

	user_cmd->pb.mutable_base( )->set_forwardmove( fixed_forward_move );
	user_cmd->pb.mutable_base( )->set_leftmove( fixed_side_move );
	user_cmd->pb.mutable_base( )->set_upmove( fixed_up_move );

	fixed_forward_move = sign * ( old_forward.dot( right ) + old_forward.dot( forward ) );
	fixed_side_move = old_right.dot( right ) + old_right.dot( forward );

	user_cmd->pb.mutable_base( )->set_forwardmove( std::clamp( fixed_forward_move, -1.f, 1.f ) );
	user_cmd->pb.mutable_base( )->set_leftmove( std::clamp( fixed_side_move, -1.f, 1.f ) );
}

void c_movement::on_create_move( c_user_cmd* user_cmd, float old_yaw ) {
	if ( g_ctx->m_local_pawn->m_move_type( ) == movetype_ladder || g_ctx->m_local_pawn->m_move_type( ) == movetype_noclip )
		return;

	quick_peek_assistant(user_cmd);
	quick_stop(user_cmd, g_ctx->m_local_pawn);
	draw_autopeek();
	bunnyhop( user_cmd );
	auto_strafe();
}