#pragma once

#include <vector>
#include <memory>
#include <optional>

#include "hittable.h"
#include "material.h"
#include "utils.h"
#include "hitrec.h"
#include "ray.h"


class HittableList {
private:
	std::vector<std::shared_ptr<Hittable>> objects{};
public:
	HittableList() = default;
	void clear();
	void add(std::shared_ptr<Hittable> hittable);
	HitScatterRecord hit(const Ray & ray, float rayTMin, float rayTMax) const;
};