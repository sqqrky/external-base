#ifndef AIMBOT_HPP
#define AIMBOT_HPP

#include <memory>

class c_aimbot
{
	std::mutex m_mutex{};

public:

	struct projectile_properties {
		int speed;
		double gravity;
	};

	uemath::fvector prediction_calculations(uemath::fvector target, uemath::fvector velocity, uemath::fvector local_velocity, float projectile_speed_scale, float projectile_gravity_scale, float distance);
	uemath::frotator find_look_at_rotation(uemath::fvector& start, uemath::fvector& target);
	uemath::frotator clamp_angle(uemath::frotator start_rot, uemath::frotator end_rot, float smoothing_scale);
	void aim_lock(uemath::fvector aim_location);

	double get_cross_distance(double x, double y);

}; inline auto aimbot = std::make_unique<c_aimbot>();

#endif