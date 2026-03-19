#pragma once
// stl
#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>
// external libs
#include <cuda/std/optional>
#include <cuda_runtime.h>
#include <glm/glm.hpp>
// my libs
#include "ray/ray.h"
#include "hitrec/hitrec.h"
#include "material/material.h"


template<typename T>
__host__ __device__ int sign(T val) {
	auto sign = (T(0) < val) - (T(0) > val);
	return sign;
}


class Hittable {
protected:
	glm::vec3 center{};
public:
	__device__ virtual ~Hittable() = default;
	__device__ virtual cuda::std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const = 0;
	__device__ virtual HitRecord constructHitRecord(const Ray& ray, float t) const = 0;
	__device__ virtual Material* getMaterial() const = 0;
	__device__ virtual void setMaterial(Material* mat) = 0;
	__device__ virtual glm::vec3 getCenter() const = 0;
};


class Sphere : public Hittable {
protected:
	Material* material = nullptr;
	glm::vec3 center{};
	float radius{};
public:
	__device__ ~Sphere() = default;
	__device__ explicit Sphere(const glm::vec3& center, float radius, Material* mat) : Hittable(), center(center), radius(fmaxf(0.0f, radius)), material(mat) {};
	__device__ virtual cuda::std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const override;
	__device__ virtual HitRecord constructHitRecord(const Ray& ray, float t) const override;
	__device__ virtual void setMaterial(Material* mat) override;
	__device__ virtual Material* getMaterial() const override;
	__device__ virtual glm::vec3 getCenter() const override;
};


class Triangle : public Hittable {
protected:
	Material* material = nullptr;
	glm::vec3 v0{}, v1{}, v2{};
	glm::vec3 normal{};
public:
	__device__ ~Triangle() = default;
	__device__ explicit Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, Material* mat) : Hittable(), v0(v0), v1(v1), v2(v2), material(mat) {
		normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
	};
	__device__ virtual cuda::std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const override;
	__device__ virtual HitRecord constructHitRecord(const Ray& ray, float t) const override;
	__device__ virtual void setMaterial(Material* mat) override;
	__device__ virtual Material* getMaterial() const override;
	__device__ virtual glm::vec3 getCenter() const override;
};