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
	uint8_t axis{};
public:
	__device__ bool isLeaf() const {
		return triangleCount > 0;
	}
};

struct BuildNode {
	AABB bbox{};
	BuildNode* left{};
	BuildNode* right{};
	int splitAxis{}; // 0 = x, 1 = y, 2 = z
	int triangleOffset{};
	int triangleCount{};

	void initLeaf(int offset, int count, const AABB& bbox) {
		triangleOffset = offset;
		triangleCount = count;
		this->bbox = bbox;
		left = right = nullptr;
	}

	void initInner(int axis, BuildNode* left, BuildNode* right) {
		splitAxis = axis;
		this->left = left;
		this->right = right;
		bbox = left->bbox;
		bbox.expand(right->bbox);
	}

	void destroy() {
		if (left) {
			left->destroy();
			delete left;
		}
		if (right) {
			right->destroy();
			delete right;
		}
	}
};

struct Bin {
	AABB bounds{};
	int count{};
};

class BVHBuilder {
private:
	int totalNodes = 0;
	BuildNode* recursiveBuild(std::vector<TriangleInfo>& triangles, int start, int end, int& nodeCount) {
		nodeCount++;
		BuildNode* node = new BuildNode();
		AABB nodeBounds{};
		AABB centroidBbox{};
		for (int i = start; i < end; i++) {
			nodeBounds.expand(triangles[i].bbox);
			centroidBbox.expand(AABB(triangles[i].centroid, triangles[i].centroid));
		}
		node->bbox = nodeBounds;
		if (end - start <= 2) {
			node->initLeaf(start, end - start, nodeBounds);
			return node;
		}
		int axis = centroidBbox.longestAxis();
		const int nBins = 12;
		Bin bins[nBins];
		for (int i = start; i < end; i++) {
			int b{};
			if (centroidBbox.getMax()[axis] != centroidBbox.getMin()[axis]) {
				b = nBins * ((triangles[i].centroid[axis] - centroidBbox.getMin()[axis]) / (centroidBbox.getMax()[axis] - centroidBbox.getMin()[axis]));
			}
			if (b == nBins) b = nBins - 1;
			bins[b].count++;
			bins[b].bounds.expand(triangles[i].bbox);
		}

		float cost[nBins - 1];
		for (int i = 0; i < nBins - 1; i++) {
			AABB b0{}, b1{};
			int count0 = 0, count1 = 0;
			for (int j = 0; j <= i; j++) {
				b0.expand(bins[j].bounds);
				count0 += bins[j].count;
			}
			for (int j = i + 1; j < nBins; j++) {
				b1.expand(bins[j].bounds);
				count1 += bins[j].count;
			}
			cost[i] = 0.125f + (count0 * b0.surfaceArea() + count1 * b1.surfaceArea()) / nodeBounds.surfaceArea();
		}
		float minCost = cost[0];
		int minCostSplit = 0;
		for (int i = 1; i < nBins - 1; i++) {
			if (cost[i] < minCost) {
				minCost = cost[i];
				minCostSplit = i;
			}
		}
		auto midPtr = std::partition(&triangles[start], &triangles[end - 1] + 1, [=](const TriangleInfo& t) {
				int b = nBins * ((t.centroid[axis] - centroidBbox.getMin()[axis]) / (centroidBbox.getMax()[axis] - centroidBbox.getMin()[axis]));
				if (b == nBins) b = nBins - 1;
				return b <= minCostSplit;
			});
		int mid = std::distance(&triangles[0], midPtr);
		if (mid == start || mid == end) {
			mid = start + (end - start) / 2;
			std::nth_element(&triangles[start], &triangles[mid], &triangles[end - 1] + 1, [=](const TriangleInfo& a, const TriangleInfo& b) {
				return a.centroid[axis] < b.centroid[axis];
				});
		}
		node->initInner(axis, recursiveBuild(triangles, start, mid, nodeCount), recursiveBuild(triangles, mid, end, nodeCount));
		return node;
	}

public:
	BVHBuilder() = default;
	BuildNode* build(std::vector<TriangleInfo>& triangles) {
		totalNodes = 0;
		return recursiveBuild(triangles, 0, triangles.size(), totalNodes);
	};
	int flattenBVH(BuildNode* node, std::vector<LinearBVHNode>& flatNodes, int& offset) {
		int currentIdx = offset++;
		flatNodes[currentIdx].bbox = node->bbox;
		if (node->triangleCount > 0) {
			flatNodes[currentIdx].offset = node->triangleOffset;
			flatNodes[currentIdx].triangleCount = node->triangleCount;
		}
		else {
			flattenBVH(node->left, flatNodes, offset);
			flatNodes[currentIdx].triangleCount = 0;
			flatNodes[currentIdx].axis = node->splitAxis;
			int rightChildIdx = flattenBVH(node->right, flatNodes, offset);
			flatNodes[currentIdx].offset = rightChildIdx;
		}
		return currentIdx;
	}
	int getTotalNodes() const {
		return totalNodes;
	}
};
