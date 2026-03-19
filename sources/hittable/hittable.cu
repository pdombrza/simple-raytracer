#include "hittable.h"


__device__ HitRecord Sphere::constructHitRecord(const Ray& ray, float t) const {
	HitRecord rec{};
	rec.t = t;
	rec.p = ray.At(t);
	glm::vec3 outwardNormal = glm::normalize(rec.p - center);
	outwardNormal *= sign(radius);
	rec.setFaceNormal(ray, outwardNormal);
	return rec;
}

__device__ cuda::std::optional<HitRecord> Sphere::hit(const Ray& ray, float rayTMin, float rayTMax) const {
	glm::vec3 distOc = center - ray.getOrigin();
	float a = glm::dot(ray.getDirection(), ray.getDirection());
	float halfb = glm::dot(ray.getDirection(), distOc);
	float c = glm::dot(distOc, distOc) - radius * radius;
	auto discriminant = halfb * halfb - a * c;
	if (discriminant < 0) return {};
	
	float sqrtDiscriminant = sqrtf(discriminant);
	float root = (halfb - sqrtDiscriminant) / a;
	if (root <= rayTMin || root >= rayTMax) {
		root = (halfb + sqrtDiscriminant) / a;
		if (root < rayTMin || root >= rayTMax) {
			return {};
		}
	}
	HitRecord record = constructHitRecord(ray, root);
	return record;
}

__device__ void Sphere::setMaterial(Material* mat) {
	material = mat;
}

__device__ Material* Sphere::getMaterial() const {
	return material;
}

__device__ glm::vec3 Sphere::getCenter() const {
	return center;
}

__device__ HitRecord Triangle::constructHitRecord(const Ray& ray, float t) const {
	HitRecord rec{};
	rec.t = t;
	rec.p = ray.At(t);
	rec.setFaceNormal(ray, normal);
	return rec;
}

__device__ cuda::std::optional<HitRecord> Triangle::hit(const Ray& ray, float rayTMin, float rayTmax) const {
	// Moller-Trumbore intersection algorithm
	glm::vec3 e0 = v1 - v0;
	glm::vec3 e1 = v2 - v0;
	glm::vec3 tvec = ray.getOrigin() - v0;
	glm::vec3 pvec = glm::cross(ray.getDirection(), e1);
	glm::vec3 qvec = glm::cross(tvec, e0);
	float denominator = glm::dot(pvec, e0);
	if (fabsf(denominator) < 1e-8f)
		return {};
	float u = glm::dot(pvec, tvec) / denominator;
	if (u < 0.0f || u > 1.0f)
		return {};
	float v = glm::dot(qvec, ray.getDirection()) / denominator;
	if (v < 0.0f || u + v > 1.0f)
		return {};
	float t = glm::dot(qvec, e1) / denominator;
	if (t < rayTMin || t >= rayTmax)
		return {};
	HitRecord record = constructHitRecord(ray, t);
	return record;
}

__device__ void Triangle::setMaterial(Material* mat) {
	material = mat;
}

__device__ Material* Triangle::getMaterial() const {
	return material;
}

__device__ glm::vec3 Triangle::getCenter() const {
	return (v0 + v1 + v2) / 3.0f;
}