#pragma once

#include <vector>
#include <memory>
#include <optional>

#include "hittable.h"
#include "hitrec.h"
#include "ray.h"


class HittableList {
private:
	std::vector<std::shared_ptr<Hittable>> objects{};
public:
	HittableList() = default;
	void clear();
	void add(std::shared_ptr<Hittable> hittable);
	std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const;
};