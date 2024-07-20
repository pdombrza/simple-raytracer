#pragma once

#include <optional>
#include <glm/glm.hpp>
#include "hitrec.h"
#include "ray.h"
#include "utils.h"
#include "scattering_record.h"


class Material {
public:
	virtual ~Material() = default;
	virtual std::optional<ScatteringRecord> scatter(const Ray& ray_in, const HitRecord& hitrec) const = 0;
};


class Lambertian : public Material {
private:
	glm::vec3 albedo;
public:
	explicit Lambertian(const glm::vec3& albedo) : albedo(albedo) {};
	std::optional<ScatteringRecord> scatter(const Ray& ray_in, const HitRecord& hitrec) const override;
};