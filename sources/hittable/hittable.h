#pragma once

#include <algorithm>
#include <optional>
#include <cmath>
#include <memory>
#include <glm/glm.hpp>
#include "ray.h"
#include "hitrec.h"
#include "material.h"



template<typename T>
int sign(T val) {
	auto sign = (T(0) < val) - (T(0) > val);
	return sign;
}


class Hittable {
public:
	virtual ~Hittable() = default;
	virtual std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const = 0;
	virtual HitRecord constructHitRecord(const Ray& ray, float t) const = 0;
};


class Sphere : public Hittable {
private:
	glm::vec3 center{};
	float radius{};
	std::unique_ptr<Material> material{}; // Maybe make shared_ptr since there may be a lot of hittables using the same material...
public:
	~Sphere() = default;
	explicit Sphere(const glm::vec3& center, float radius) : center(center), radius(std::max(0.0f, radius)) {};
	std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const;
	HitRecord constructHitRecord(const Ray& ray, float t) const;
	void setMaterial(std::unique_ptr<Material> mat);
};