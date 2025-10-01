#include <glm/glm.hpp>

#include "hittablelist.h"


__device__ void HittableList::clear() {
	objCount = 0;
}

__device__ void HittableList::add(Hittable* hittable) {
	if (objCount < capacity) {
		objects[objCount] = hittable;
		objCount++;
	}
}

__device__ HitScatterRecord HittableList::hit(const Ray& ray, float rayTMin, float rayTMax) const {
	HitRecord closestHit;
	HitScatterRecord HSRec{};
	float closestDist = rayTMax;
	bool hitAnything = false;
	Hittable* closestObj = nullptr;
	
	for (int i = 0; i < objCount; i++) {
		Hittable* object = objects[i];
		cuda::std::optional<HitRecord> tempRec = object->hit(ray, rayTMin, rayTMax);
		if (tempRec.has_value() && tempRec.value().t < closestDist) {
			hitAnything = true;
			closestObj = object;
			closestDist = tempRec.value().t;
			closestHit = tempRec.value();
		}
	}
	if (!hitAnything) return HSRec;
	HSRec.hitRec = closestHit;
	/*std::optional<ScatteringRecord> sRec = closestObj->getMaterial()->scatter(ray, closestHit);
	HSRec.scatterRec = sRec;*/

	return HSRec;
}