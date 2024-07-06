#pragma once

#include "hitrec.h"
#include "ray.h"

class Material {
public:
	virtual ~Material() = default;
	virtual Ray scatter(const Ray& ray_in, const HitRecord& hitrec, glm::vec3& attenuation) const = 0;
};


