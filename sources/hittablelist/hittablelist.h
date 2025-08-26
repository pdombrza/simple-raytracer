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
	std::vector<std::shared_ptr<Hittable>> objects{};
public:
	HittableList() = default;
	void clear();
	void add(std::shared_ptr<Hittable> hittable);
	HitScatterRecord hit(const Ray & ray, float rayTMin, float rayTMax) const;
};