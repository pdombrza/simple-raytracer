#pragma once

#include <algorithm>
#include <optional>
#include <cmath>
#include <memory>
#include <utility>
#include <glm/glm.hpp>
#include "ray.h"
#include "hitrec.h"
#include "utils.h"
#include "material.h"


template<typename T>
int sign(T val) {
	auto sign = (T(0) < val) - (T(0) > val);
	return sign;
}


class Hittable {
protected:
	glm::vec3 center{};
public:
	virtual ~Hittable() = default;
	virtual std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const = 0;
	virtual HitRecord constructHitRecord(const Ray& ray, float t) const = 0;
	virtual std::shared_ptr<Material> getMaterial() const = 0;
	virtual void setMaterial(std::shared_ptr<Material> mat) = 0;
	virtual glm::vec3 getCenter() const = 0;
};


class Sphere : public Hittable {
protected:
	std::shared_ptr<Material> material{};
	glm::vec3 center{};
	float radius{};
public:
	~Sphere() = default;
	explicit Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> mat) : Hittable(), center(center), radius(std::max(0.0f, radius)), material(mat) {};
	virtual std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const override;
	virtual HitRecord constructHitRecord(const Ray& ray, float t) const override;
	virtual void setMaterial(std::shared_ptr<Material> mat) override;
	virtual std::shared_ptr<Material> getMaterial() const override;
	virtual glm::vec3 getCenter() const override;
};