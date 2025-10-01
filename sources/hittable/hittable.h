#pragma once
// stl
#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>
// cuda
#include <cuda/std/optional>
#include <cuda_runtime.h>
// my libs
#include "ray/ray.h"
#include "utils/utils.h"
#include "hitrec/hitrec.h"
#include "material/material.h"
#include "cuvec/cuvec.h"


template<typename T>
__host__ __device__ int sign(T val) {
	auto sign = (T(0) < val) - (T(0) > val);
	return sign;
}


class Hittable {
protected:
	cu::vec3 center{};
public:
	__device__ virtual ~Hittable() = default;
	__device__ virtual cuda::std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const = 0;
	__device__ virtual HitRecord constructHitRecord(const Ray& ray, float t) const = 0;
	//__device__ virtual std::shared_ptr<Material> getMaterial() const = 0;
	//__device__ virtual void setMaterial(std::shared_ptr<Material> mat) = 0;
	__device__ virtual cu::vec3 getCenter() const = 0;
};


class Sphere : public Hittable {
protected:
	//std::shared_ptr<Material> material{};
	cu::vec3 center{};
	float radius{};
public:
	__device__ ~Sphere() = default;
	__device__ explicit Sphere(const cu::vec3& center, float radius) : Hittable(), center(center), radius(fmaxf(0.0f, radius)) {};
	__device__ virtual cuda::std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const override;
	__device__ virtual HitRecord constructHitRecord(const Ray& ray, float t) const override;
	//virtual void setMaterial(std::shared_ptr<Material> mat) override;
	//virtual std::shared_ptr<Material> getMaterial() const override;
	__device__ virtual cu::vec3 getCenter() const override;
};