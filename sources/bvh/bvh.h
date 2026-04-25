#pragma once
#include <glm/glm.hpp>

#include "aabb/aabb.h"


struct TriangleInfo {
	int triangleIndex{};
	AABB bbox{};
	glm::vec3 centroid{};
	
	TriangleInfo(int id, const AABB& bbox) : triangleIndex(id), bbox(bbox) {
		centroid = (bbox.getMin() + bbox.getMax()) * 0.5f;
	}
};

class LinearBVHNode {
public:
	AABB bbox{};
	int32_t offset{};
	uint16_t triangleCount{};
	uint8_t axis{}
public:
	bool isLeaf() const {
		return triangleCount > 0;
	}
};

class BVHBuilder {
private:
	std::vector<glm::vec3> h_vertices{};
	std::vector<int> h_indices{};
public:
	BVHBuilder(const std::vector<glm::vec3>& vertices, const std::vector<int>& indices);
	std::vector<BVHNode> build();
};