#include <glm/glm.hpp>

#include "hittablelist.h"


void HittableList::clear() {
	objects.clear();
}

void HittableList::add(std::shared_ptr<Hittable> hittable) {
	objects.push_back(hittable);
}

HitScatterRecord HittableList::hit(const Ray& ray, float rayTMin, float rayTMax) const {
	HitRecord closestHit;
	HitScatterRecord HSRec{};
	float closestDist = rayTMax;
	bool hitAnything = false;
	std::shared_ptr<Hittable> closestObj = nullptr;
	
	for (const auto& object : objects) {
		std::optional<HitRecord> tempRec = object->hit(ray, rayTMin, rayTMax);
		if (tempRec.has_value() && tempRec.value().t < closestDist) {
			hitAnything = true;
			closestObj = object;
			closestDist = tempRec.value().t;
			closestHit = tempRec.value();
		}
	}
	if (!hitAnything) return HSRec;
	HSRec.hitRec = closestHit;
	std::optional<ScatteringRecord> sRec = closestObj->getMaterial()->scatter(ray, closestHit);
	HSRec.scatterRec = sRec;

	return HSRec;
}