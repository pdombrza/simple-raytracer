#include <glm/glm.hpp>

#include "hittablelist.h"


void HittableList::clear() {
	objects.clear();
}

void HittableList::add(std::shared_ptr<Hittable> hittable) {
	objects.push_back(hittable);
}

std::optional<HitRecord> HittableList::hit(const Ray& ray, float rayTMin, float rayTMax) const {
	HitRecord rec{};
	float closestDist = rayTMax;
	HitRecord* closestRecord = nullptr;
	
	for (const auto& object : objects) {
		std::optional<HitRecord> tempRec = object->hit(ray, rayTMin, rayTMax);
		if (tempRec.has_value()) {
			closestDist = tempRec.value().t;
			closestRecord = &tempRec.value();
		}
	}
	if (!closestRecord) return {};
	return *closestRecord;
}