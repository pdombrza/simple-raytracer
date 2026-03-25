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
	__host__ __device__ virtual ~Hittable() = default;
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
	__host__ __device__ ~Sphere() = default;
	__host__ __device__ explicit Sphere(const glm::vec3& center, float radius, Material* mat) : Hittable(), center(center), radius(fmaxf(0.0f, radius)), material(mat) {};
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
	__host__ __device__ ~Triangle() = default;
	__host__ __device__ explicit Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, Material* mat) : Hittable(), v0(v0), v1(v1), v2(v2), material(mat) {
		normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
	};
	__device__ virtual cuda::std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const override;
	__device__ virtual HitRecord constructHitRecord(const Ray& ray, float t) const override;
	__device__ virtual void setMaterial(Material* mat) override;
	__device__ virtual Material* getMaterial() const override;
	__device__ virtual glm::vec3 getCenter() const override;
};

__device__ static HitRecord constructHitRecord(const Ray& ray, float t, const glm::vec3& normal) {
	HitRecord rec{};
	rec.t = t;
	rec.p = ray.At(t);
	rec.setFaceNormal(ray, normal);
	return rec;
};

__device__ static cuda::std::optional<HitRecord> rayTriangleIntersection(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, const Ray& ray, float rayTMin, float rayTMax) {
	// Moller-Trumbore intersection algorithm 
	glm::vec3 e0 = v1 - v0;
	glm::vec3 e1 = v2 - v0;
	glm::vec3 tvec = ray.getOrigin() - v0;
	glm::vec3 pvec = glm::cross(ray.getDirection(), e1);
	glm::vec3 qvec = glm::cross(tvec, e0);
	float denominator = glm::dot(pvec, e0);
	if (fabsf(denominator) < 1e-8f)
		return {};
	float u = glm::dot(pvec, tvec) / denominator;
	if (u < 0.0f || u > 1.0f)
		return {};
	float v = glm::dot(qvec, ray.getDirection()) / denominator;
	if (v < 0.0f || u + v > 1.0f)
		return {};
	float t = glm::dot(qvec, e1) / denominator;
	if (t < rayTMin || t >= rayTMax)
		return {};
	glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
	HitRecord record = constructHitRecord(ray, t, normal);
	return record;
};