#ifndef G_VARS_HPP
#define G_VARS_HPP

#include <locale>

class vars {
public:
	struct {
		bool enabled = true;
		bool prediction = true;
		float fov = 150.0f;
		float smoothing = 1.75f;
		bool ignore_invisible = false;
		bool ignore_knocked = false;
		bool ignore_dying = false;

		struct {
			bool dot = false;
			bool line = false;
			bool fov = false;
			bool crosshair = false;
		} display;

		struct {
			bool enabled = true;
			bool always = true;
			bool shotgun = true;
			float ms_delay = 0.0f;
			float max_distance = 20.0f;
		} triggerbot;

	} aimbot;

	struct {
		bool box = true;
		bool bounding_box = false;
		bool cornered_box = false;
		bool three_dimensional_box = false;
		int box_thickness = 1.25f;
		bool distance = true;
		bool weapon = true;
		bool ammo = true;
		bool username = true;
		bool skeleton = false;
		bool rank = false;

		struct {
			bool filled = true;
			bool outlined = true;
			bool outlined_text = true;
		} other;

		struct {
			bool weapon = false;
			bool chest = false;
			bool llama = false;
			bool ammo = false;
			bool vehicle = false;

			int max_distance = 50.0f;
		} floor;

		struct {
			bool enabled = false;
			int style = 0.0f;
			bool distance = false;
			bool outlined_text = false;
			bool rings = false;
			bool gridded = false;
			bool cross = true;
			int alpha = 200.0f;
			int radar_position_x = 10.0f;
			int radar_position_y = 10.0f;
			int radar_size = 150.0f;
		} radar;

		float max_distance = 200.0f;
	} visuals;

	struct {
		bool vsync = true;
		bool render_menu = false;

		struct {
			bool fps = true;
		} watermark;
	} misc;

}; inline std::unique_ptr<vars> g_vars = std::make_unique<vars>();

#endif