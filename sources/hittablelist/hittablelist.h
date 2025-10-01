#pragma once

#include <vector>
#include <memory>
#include <optional>

#include "hittable/hittable.h"
#include "material/material.h"
#include "utils/utils.h"
#include "hitrec/hitrec.h"
#include "ray/ray.h"


class HittableList {
private:
	Hittable** objects = nullptr;
	int capacity = 0;
	int objCount = 0;
public:
	__device__ HittableList() = default;
	__device__ HittableList(Hittable** objectArray, int capacity) : objects(objectArray), capacity(capacity) {};
	__device__ void clear();
	__device__ void add(Hittable* hittable);
	__device__ HitScatterRecord hit(const Ray& ray, float rayTMin, float rayTMax) const;
};