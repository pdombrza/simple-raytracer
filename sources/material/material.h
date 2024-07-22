#pragma once

#include <optional>
#include <algorithm>
#include <glm/glm.hpp>
#include "hitrec.h"
#include "ray.h"
#include "utils.h"
#include "scattering_record.h"


class Material {
public:
	virtual ~Material() = default;
	virtual std::optional<ScatteringRecord> scatter(const Ray& rayIn, const HitRecord& hitrec) const = 0;
};


class Lambertian : public Material {
private:
	glm::vec3 albedo{};
public:
	explicit Lambertian(const glm::vec3& albedo) : albedo(albedo) {};
	std::optional<ScatteringRecord> scatter(const Ray& ray_in, const HitRecord& hitrec) const override;
};


class Metal : public Material {
private:
	glm::vec3 albedo{};
	float fuzziness{1.0f};
public:
	explicit Metal(const glm::vec3& albedo, float fuzziness) : albedo(albedo), fuzziness(std::min(1.0f, fuzziness)) {};
	std::optional<ScatteringRecord> scatter(const Ray& rayIn, const HitRecord& hitrec) const override;
};


class Dielectric : public Material {
private:
	float refractionIndex{};
public:
	explicit Dielectric(float refractionIndex) : refractionIndex(refractionIndex) {};
	std::optional<ScatteringRecord> scatter(const Ray& rayIn, const HitRecord& hitrec) const override;
};