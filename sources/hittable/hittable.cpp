#include "hittable.h"


HitRecord Sphere::constructHitRecord(const Ray& ray, float t) const {
	HitRecord rec{};
	rec.t = t;
	rec.p = ray.At(t);
	glm::vec3 outwardNormal = glm::normalize(rec.p - center);
	outwardNormal *= sign(radius);
	rec.setFaceNormal(ray, outwardNormal);
	return rec;
}

std::optional<HitRecord> Sphere::hit(const Ray& ray, float rayTMin, float rayTMax) const {
	glm::vec3 distOC = center - ray.getOrigin();
	float a = glm::dot(ray.getDirection(), ray.getDirection());
	float halfb = glm::dot(ray.getDirection(), distOC);
	float c = glm::dot(distOC, distOC) - radius * radius;
	float delta = halfb * halfb - a * c;
	if (delta < 0)
		return {};

	float sqrtDelta = std::sqrt(delta);
	float root = (halfb - sqrtDelta) / a;
	if (root < rayTMin || root >= rayTMax) {
		root = (halfb + sqrtDelta) / a;
		if (root < rayTMin || root >= rayTMax) {
			return {};
		}
	}
	HitRecord record = constructHitRecord(ray, root);
	return record;
}


void Sphere::setMaterial(std::unique_ptr<Material> mat) {
	material = std::move(mat);
}