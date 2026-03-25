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

__device__ HitScatterRecord HittableList::hit(const Ray& ray, float rayTMin, float rayTMax, utils::random::RNG& rng) const {
	HitRecord closestHit;
	HitScatterRecord HSRec{};
	float closestDist = rayTMax;
	bool hitAnything = false;
	Material* closestMat = nullptr;
	
	for (int i = 0; i < objCount; i++) {
		Hittable* object = objects[i];
		cuda::std::optional<HitRecord> tempRec = object->hit(ray, rayTMin, closestDist);
		if (tempRec.has_value() && tempRec.value().t < closestDist) {
			hitAnything = true;
			closestMat = object->getMaterial();
			closestDist = tempRec.value().t;
			closestHit = tempRec.value();
		}
	}

	for (int i = 0; i < meshCount; i++) {
		auto tempRec = meshes[i].hit(ray, rayTMin, closestDist);
		if (tempRec.has_value() && tempRec.value().t < closestDist) {
			hitAnything = true;
			closestMat = meshes[i].getMaterial();
			closestDist = tempRec.value().t;
			closestHit = tempRec.value();
		}
	}

	if (!hitAnything) return HSRec;
	HSRec.hitRec = closestHit;
	cuda::std::optional<ScatteringRecord> sRec = closestMat->scatter(ray, closestHit, rng);
	HSRec.scatterRec = sRec;

	return HSRec;
}