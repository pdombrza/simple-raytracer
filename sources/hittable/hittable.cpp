#include "hittable.h"


HitRecord Sphere::constructHitRecord(const Ray& ray, float t) const {
	HitRecord rec{};
	rec.t = t;
	rec.p = ray.At(t);
	rec.normal = glm::normalize(rec.p - center);
	rec.normal = rec.normal * radius;
	return rec;
}

std::optional<HitRecord> Sphere::testHit(const Ray& ray, float rayTMin, float rayTMax) const {
	glm::vec3 distOC = center - ray.getOrigin();
	float a = glm::dot(ray.getDirection(), ray.getDirection());
	float h = glm::dot(ray.getDirection(), distOC);
	float c = glm::dot(distOC, distOC) - radius * radius;
	float delta = h * h - a * c;
	if (delta < 0)
		return {};

	float sqrtDelta = sqrt(delta);
	float root = (h - sqrtDelta) / a;
	if (root < rayTMin || root >= rayTMax) {
		root = (h + sqrtDelta) / a;
		if (root < rayTMin || root >= rayTMax) {
			return {};
		}
	}
	HitRecord record = constructHitRecord(ray, root);
	return record;
}

