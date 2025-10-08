#pragma once

#include <vector>
#include <memory>
#include <optional>

#include "hittable/hittable.h"
#include "material/material.h"
#include "hitrec/hitrec.h"
#include "ray/ray.h"


class HittableList {
private:
	Hittable** objects = nullptr;
	int objCount = 0;
	int capacity = 0;
public:
	__device__ HittableList() = default;
	__device__ HittableList(Hittable** objectArray, int numObjects, int capacity) : objects(objectArray), objCount(numObjects), capacity(capacity) {};
	__device__ void clear();
	__device__ void add(Hittable* hittable);
	__device__ HitScatterRecord hit(const Ray& ray, float rayTMin, float rayTMax, utils::random::RNG& rng) const;
	__device__ int getObjCount() const { return objCount; };
	__device__ int getCapacity() const { return capacity; };
};