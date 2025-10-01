#include "hittable.h"


__device__ HitRecord Sphere::constructHitRecord(const Ray& ray, float t) const {
	HitRecord rec{};
	rec.t = t;
	rec.p = ray.At(t);
	cu::vec3 outwardNormal = cu::normalize(rec.p - center);
	outwardNormal *= sign(radius);
	rec.setFaceNormal(ray, outwardNormal);
	return rec;
}

__device__ cuda::std::optional<HitRecord> Sphere::hit(const Ray& ray, float rayTMin, float rayTMax) const {
	cu::vec3 distOc = center - ray.getOrigin();
	float a = ray.getDirection().lengthSquared();
	float halfb = cu::dot(ray.getDirection(), distOc);
	float c = distOc.lengthSquared() - radius * radius;
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

//void Sphere::setMaterial(std::shared_ptr<Material> mat) {
//	material = mat;
//}
//
//std::shared_ptr<Material> Sphere::getMaterial() const {
//	return material;
//}

__device__ cu::vec3 Sphere::getCenter() const {
	return center;
}
