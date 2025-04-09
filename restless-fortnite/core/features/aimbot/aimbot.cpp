#ifndef AIMBOT_CPP
#define AIMBOT_CPP

#include <core/includes.hpp>

auto c_aimbot::prediction_calculations(uemath::fvector target, uemath::fvector velocity, uemath::fvector local_velocity, float projectile_speed_scale, float projectile_gravity_scale, float distance) -> uemath::fvector {
}

uemath::frotator c_aimbot::find_look_at_rotation(uemath::fvector& start, uemath::fvector& target) {
}

uemath::frotator c_aimbot::clamp_angle(uemath::frotator start_rot, uemath::frotator end_rot, float smoothing_scale) {
}

auto c_aimbot::aim_lock(uemath::fvector aim_location) -> void {
}

double c_aimbot::get_cross_distance(double x, double y) {
}

#endif