#include "cuvec.h"

__host__ __device__ cu::vec3::vec3(float x, float y, float z) {
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

__host__ __device__ cu::vec3& cu::vec3::operator+=(const cu::vec3& v) {
	data[0] += v.x();
	data[1] += v.y();
	data[2] += v.z();
	return *this;
}

__host__ __device__ cu::vec3& cu::vec3::operator-=(const cu::vec3& v) {
	data[0] -= v.x();
	data[1] -= v.y();
	data[2] -= v.z();
	return *this;
}

__host__ __device__ cu::vec3& cu::vec3::operator*=(const cu::vec3& v) {
	data[0] *= v.x();
	data[1] *= v.y();
	data[2] *= v.z();
	return *this;
}

__host__ __device__ cu::vec3& cu::vec3::operator/=(const cu::vec3& v) {
	data[0] /= v.x();
	data[1] /= v.y();
	data[2] /= v.z();
	return *this;
}

__host__ __device__ cu::vec3& cu::vec3::operator*=(float t) {
	data[0] *= t;
	data[1] *= t;
	data[2] *= t;
	return *this;
}

__host__ __device__ cu::vec3& cu::vec3::operator/=(float t) {
	float k = 1.0f / t;
	data[0] *= k;
	data[1] *= k;
	data[2] *= k;
	return *this;
}

__host__ __device__ float cu::vec3::length() const {
	return sqrtf(dot(*this, *this));
}

__host__ __device__ float cu::vec3::length_squared() const {
	return dot(*this, *this);
}

__host__ __device__ cu::vec3 cu::normalize(const vec3& v) {
	float vectorLength = v.length();
	return cu::vec3(v.x() / vectorLength, v.y() / vectorLength, v.z() / vectorLength);
}

__host__ __device__ cu::vec3 cu::operator+(const cu::vec3& v1, const cu::vec3& v2) {
	return cu::vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

__host__ __device__ cu::vec3 cu::operator-(const cu::vec3& v1, const cu::vec3& v2) {
	return cu::vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

__host__ __device__ cu::vec3 cu::operator*(const cu::vec3& v1, const cu::vec3& v2) {
	return cu::vec3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

__host__ __device__ cu::vec3 cu::operator/(const cu::vec3& v1, const cu::vec3& v2) {
	return cu::vec3(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
}

__host__ __device__ cu::vec3 cu::operator*(float t, const cu::vec3& v) {
	return cu::vec3(t * v.x(), t * v.y(), t * v.z());
}

__host__ __device__ cu::vec3 cu::operator/(const cu::vec3& v, float t) {
	return cu::vec3(v.x() / t, v.y() / t, v.z() / t);
}

__host__ __device__ float cu::dot(const cu::vec3& v1, const cu::vec3& v2) {
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

__host__ __device__ cu::vec3 cu::cross(const cu::vec3& v1, const cu::vec3& v2) {
	return cu::vec3{ v1.y() * v2.z() - v1.z() * v2.y(), v1.z() * v2.x() - v1.x() * v2.z(), v1.x() * v2.y() - v1.y() * v2.x() };
}

inline std::istream& operator>>(std::istream& is, cu::vec3& t) {
	float x, y, z;
	is >> x >> y >> z;
	t = cu::vec3(x, y, z);
	return is;
}

inline std::ostream& operator<<(std::ostream& os, const cu::vec3& t) {
	float x = t.x();
	float y = t.y();
	float z = t.z();
	os << x << " " << y << " " << z;
	return os;
}
