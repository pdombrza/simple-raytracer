#include <glm/glm.hpp>

#include "hittablelist.h"


void HittableList::clear() {
	objects.clear();
}

void HittableList::add(std::shared_ptr<Hittable> hittable) {
	objects.push_back(hittable);
}

HitScatterRecord HittableList::hit(const Ray & ray, float rayTMin, float rayTMax) const {
	HitRecord rec{};
	HitScatterRecord HSRec{};
	float closestDist = rayTMax;
	HitRecord* closestRecord = nullptr;
	std::shared_ptr<Hittable> closestObj = nullptr;
	
	for (const auto& object : objects) {
		std::optional<HitRecord> tempRec = object->hit(ray, rayTMin, rayTMax);
		if (tempRec.has_value()) {
			closestObj = object;
			closestDist = tempRec.value().t;
			closestRecord = &tempRec.value();
		}
	}
	if (!closestRecord) return HSRec;
	HSRec.hitRec = *closestRecord;
	std::optional<ScatteringRecord> sRec = closestObj->getMaterial()->scatter(ray, *closestRecord);
	HSRec.scatterRec = sRec;

	return HSRec;
}