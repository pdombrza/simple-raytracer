#pragma once
#include <array>
#include <iostream>
#include <cuda_runtime.h>

namespace cu {
	class vec3 {
	protected:
		float data[3];
	public:
		__host__ __device__ vec3() : data{ 0.0f, 0.0f, 0.0f } {}
		__host__ __device__ vec3(float x, float y, float z);
		__host__ __device__ vec3(std::initializer_list<float> list);
		__host__ __device__ inline float x() const { return data[0]; }
		__host__ __device__ inline float y() const { return data[1]; }
		__host__ __device__ inline float z() const { return data[2]; }
		__host__ __device__ inline float r() const { return x(); };
		__host__ __device__ inline float g() const { return y(); };
		__host__ __device__ inline float b() const { return z(); };

		__host__ __device__ vec3 operator-() { return vec3(-data[0], -data[1], -data[2]); };
		__host__ __device__ float operator[](int i) const { return data[i]; };
		__host__ __device__ float& operator[](int i) { return data[i]; }

		__host__ __device__ vec3& operator+=(const vec3& v);
		__host__ __device__ vec3& operator-=(const vec3& v);
		__host__ __device__ vec3& operator*=(const vec3& v);
		__host__ __device__ vec3& operator/=(const vec3& v);

		__host__ __device__ vec3& operator*=(float t);
		__host__ __device__ vec3& operator/=(float t);

		__host__ __device__ float length() const;
		__host__ __device__ float lengthSquared() const;

	};
	__host__ __device__ vec3 operator+(const vec3& v1, const vec3& v2);
	__host__ __device__ vec3 operator-(const vec3& v1, const vec3& v2);
	__host__ __device__ vec3 operator*(const vec3& v1, const vec3& v2);
	__host__ __device__ vec3 operator/(const vec3& v1, const vec3& v2);

	__host__ __device__ vec3 operator*(float t, const vec3& v);
	__host__ __device__ vec3 operator*(const vec3& v, float t);
	__host__ __device__ vec3 operator/(const vec3& v, float t);

	__host__ __device__ float dot(const vec3& v1, const vec3& v2);
	__host__ __device__ vec3 cross(const vec3& v1, const vec3& v2);
	__host__ __device__ vec3 normalize(const vec3& v);

	__host__ __device__ vec3 clamp(const vec3& v, float min, float max);
}

inline std::istream& operator>>(std::istream& is, cu::vec3& t);
inline std::ostream& operator<<(std::ostream& os, const cu::vec3& t);

