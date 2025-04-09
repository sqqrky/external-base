#pragma once 

#include <memory>
#include <vector>

#include <d3d9types.h>

namespace uengine
{
	template< class type >
	class tarray
	{
	public:
		tarray() : data(), count(), max_count() {}
		tarray(type* data, std::uint32_t count, std::uint32_t max_count) :
			data(data), count(count), max_count(max_count) {
		}

		__forceinline type get(uintptr_t idx)
		{
			return kernel->read< type >(
				std::bit_cast<std::uintptr_t>(this->data) + (idx * sizeof(type))
			);
		}

		__forceinline std::vector<type> get_itter()
		{
			if (this->count > this->max_count)
				return { };

			try {
				std::vector< type > buffer(this->count);

				kernel->read_physical(
					std::bit_cast<void*>(this->data),
					buffer.data(),
					sizeof(type) * this->count
				);

				return buffer;
			}
			catch (std::bad_alloc& e) {
				return { };
			}
		}

		__forceinline std::uintptr_t get_addr()
		{
			return reinterpret_cast<std::uintptr_t>(this->data);
		}

		__forceinline std::uint32_t size() const
		{
			return this->count;
		};

		__forceinline std::uint32_t max_size() const
		{
			return this->max_count;
		};

		__forceinline bool is_valid_index(std::int32_t index) const
		{
			return this->index >= 0 && index < count;
		};

		__forceinline bool is_valid() const
		{
			return this->data != nullptr;
		};

		type* data;
		uint32_t count;
		uint32_t max_count;
	};

	struct ftransform {
		uemath::fplane rot;
		uemath::fvector translation;
		uint8_t pad[8];
		uemath::fvector scale;
		uint8_t pad2[8];

		D3DMATRIX to_matrix_with_scale() const {
			uemath::fvector scale_3d
			(
				(scale.x == 0.0) ? 1.0 : scale.x,
				(scale.y == 0.0) ? 1.0 : scale.y,
				(scale.z == 0.0) ? 1.0 : scale.z
			);

			double x2 = rot.x + rot.x;
			double y2 = rot.y + rot.y;
			double z2 = rot.z + rot.z;
			double xx2 = rot.x * x2;
			double yy2 = rot.y * y2;
			double zz2 = rot.z * z2;
			double yz2 = rot.y * z2;
			double wx2 = rot.w * x2;
			double xy2 = rot.x * y2;
			double wz2 = rot.w * z2;
			double xz2 = rot.x * z2;
			double wy2 = rot.w * y2;

			D3DMATRIX m;

			m._41 = translation.x;
			m._42 = translation.y;
			m._43 = translation.z;

			m._11 = (1.0 - (yy2 + zz2)) * scale_3d.x;
			m._22 = (1.0 - (xx2 + zz2)) * scale_3d.y;
			m._33 = (1.0 - (xx2 + yy2)) * scale_3d.z;

			m._32 = (yz2 - wx2) * scale_3d.z;
			m._23 = (yz2 + wx2) * scale_3d.y;

			m._21 = (xy2 - wz2) * scale_3d.y;
			m._12 = (xy2 + wz2) * scale_3d.x;

			m._31 = (xz2 + wy2) * scale_3d.z;
			m._13 = (xz2 - wy2) * scale_3d.x;

			m._14 = 0.0f;
			m._24 = 0.0f;
			m._34 = 0.0f;
			m._44 = 1.0f;

			return m;
		}
	};

	inline D3DMATRIX matrix_multiplication(D3DMATRIX matrix1, D3DMATRIX matrix2)
	{
		D3DMATRIX result;
		result._11 = matrix1._11 * matrix2._11 + matrix1._12 * matrix2._21 + matrix1._13 * matrix2._31 + matrix1._14 * matrix2._41;
		result._12 = matrix1._11 * matrix2._12 + matrix1._12 * matrix2._22 + matrix1._13 * matrix2._32 + matrix1._14 * matrix2._42;
		result._13 = matrix1._11 * matrix2._13 + matrix1._12 * matrix2._23 + matrix1._13 * matrix2._33 + matrix1._14 * matrix2._43;
		result._14 = matrix1._11 * matrix2._14 + matrix1._12 * matrix2._24 + matrix1._13 * matrix2._34 + matrix1._14 * matrix2._44;
		result._21 = matrix1._21 * matrix2._11 + matrix1._22 * matrix2._21 + matrix1._23 * matrix2._31 + matrix1._24 * matrix2._41;
		result._22 = matrix1._21 * matrix2._12 + matrix1._22 * matrix2._22 + matrix1._23 * matrix2._32 + matrix1._24 * matrix2._42;
		result._23 = matrix1._21 * matrix2._13 + matrix1._22 * matrix2._23 + matrix1._23 * matrix2._33 + matrix1._24 * matrix2._43;
		result._24 = matrix1._21 * matrix2._14 + matrix1._22 * matrix2._24 + matrix1._23 * matrix2._34 + matrix1._24 * matrix2._44;
		result._31 = matrix1._31 * matrix2._11 + matrix1._32 * matrix2._21 + matrix1._33 * matrix2._31 + matrix1._34 * matrix2._41;
		result._32 = matrix1._31 * matrix2._12 + matrix1._32 * matrix2._22 + matrix1._33 * matrix2._32 + matrix1._34 * matrix2._42;
		result._33 = matrix1._31 * matrix2._13 + matrix1._32 * matrix2._23 + matrix1._33 * matrix2._33 + matrix1._34 * matrix2._43;
		result._34 = matrix1._31 * matrix2._14 + matrix1._32 * matrix2._24 + matrix1._33 * matrix2._34 + matrix1._34 * matrix2._44;
		result._41 = matrix1._41 * matrix2._11 + matrix1._42 * matrix2._21 + matrix1._43 * matrix2._31 + matrix1._44 * matrix2._41;
		result._42 = matrix1._41 * matrix2._12 + matrix1._42 * matrix2._22 + matrix1._43 * matrix2._32 + matrix1._44 * matrix2._42;
		result._43 = matrix1._41 * matrix2._13 + matrix1._42 * matrix2._23 + matrix1._43 * matrix2._33 + matrix1._44 * matrix2._43;
		result._44 = matrix1._41 * matrix2._14 + matrix1._42 * matrix2._24 + matrix1._43 * matrix2._34 + matrix1._44 * matrix2._44;

		return result;
	}

	inline D3DMATRIX create_rotation_matrix(uemath::frotator rotation)
	{
		// "if ur seeing this carter im sorry" ahh location

		auto rad_pitch = (rotation.pitch * float(std::numbers::pi) / 180.f);
		auto rad_yaw = (rotation.yaw * float(std::numbers::pi) / 180.f);
		auto rad_roll = (rotation.roll * float(std::numbers::pi) / 180.f);

		auto sp = std::sinf(rad_pitch);
		auto cp = std::cosf(rad_pitch);
		auto sy = std::sinf(rad_yaw);
		auto cy = std::cosf(rad_yaw);
		auto sr = std::sinf(rad_roll);
		auto cr = std::cosf(rad_roll);

		D3DMATRIX matrix;
		matrix.m[0][0] = cp * cy;
		matrix.m[0][1] = cp * sy;
		matrix.m[0][2] = sp;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = sr * sp * cy - cr * sy;
		matrix.m[1][1] = sr * sp * sy + cr * cy;
		matrix.m[1][2] = -sr * cp;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(cr * sp * cy + sr * sy);
		matrix.m[2][1] = cy * sr - cr * sp * sy;
		matrix.m[2][2] = cr * cp;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = 0.0f;
		matrix.m[3][1] = 0.0f;
		matrix.m[3][2] = 0.0f;
		matrix.m[3][3] = 1.f;

		return matrix;
	}

	struct alignas(16) matrix_elements {
		double m11, m12, m13, m14;
		double m21, m22, m23, m24;
		double m31, m32, m33, m34;
		double m41, m42, m43, m44;

		matrix_elements() : m11(0), m12(0), m13(0), m14(0),
			m21(0), m22(0), m23(0), m24(0),
			m31(0), m32(0), m33(0), m34(0),
			m41(0), m42(0), m43(0), m44(0) {
		}
	};

	struct alignas(16) dbl_matrix {
		union {
			matrix_elements elements;
			double m[4][4];
		};

		dbl_matrix() : elements() {}

		double& operator()(std::size_t row, std::size_t col) { return m[row][col]; }
		const double& operator()(std::size_t row, std::size_t col) const { return m[row][col]; }
	};

	struct alignas(16) fmatrix : public dbl_matrix {
		uemath::fplane x_plane;
		uemath::fplane y_plane;
		uemath::fplane z_plane;
		uemath::fplane w_plane;

		fmatrix() : dbl_matrix(), x_plane(), y_plane(), z_plane(), w_plane() {}
	};

	inline const float rad_to_deg = 180.0f / std::numbers::pi;

	void update_matrix();

	uemath::fvector get_bone_data(int bone_index, std::uintptr_t bone_space_transform, uengine::ftransform component_to_world);
	std::vector<uemath::fvector> get_all_bone_data(const int* bone_indices, size_t num_bones, uintptr_t bone_array_ptr, uengine::ftransform component_to_world);

	uemath::fvector2d world_to_screen(uemath::fvector location, D3DMATRIX rotation_matrix, float fov_type_shit);

	std::string decrypt_name(std::uintptr_t state);

	std::string get_weapon_name(std::uintptr_t current_weapon);

	std::string get_local_weapon_name();

	bool is_visible(std::uintptr_t mesh);

	class a_camera_data
	{
	public:
		uemath::fvector location;
		uemath::frotator rotation;
		float fov;
	}; inline std::unique_ptr<a_camera_data> camera_data = std::make_unique<a_camera_data>();

	inline float speed;
	inline float gravity;
}