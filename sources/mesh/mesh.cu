#include "mesh/mesh.h"


__device__ cuda::std::optional<HitRecord> Mesh::hit(const Ray& ray, float rayTMin, float rayTMax) const {
	cuda::std::optional<HitRecord> closestHit = {};
	float closestDist = rayTMax;
	int stack[64];
	int stackPtr = 0;
	stack[stackPtr++] = 0;
	while (stackPtr > 0) {
		int nodeIdx = stack[--stackPtr];
		const LinearBVHNode& node = nodes[nodeIdx];
		if (!node.bbox.hit(ray, rayTMin, closestDist)) {
			continue;
		}
		if (node.isLeaf()) {
			for (size_t i = 0; i < node.triangleCount; i++) {
				int idx = (node.offset + i) * 3;
				cuda::std::optional<HitRecord> hitrec = rayTriangleIntersection(vertexArray[indexArray[idx]], vertexArray[indexArray[idx + 1]], vertexArray[indexArray[idx + 2]], ray, rayTMin, closestDist);
				if (hitrec.has_value() && hitrec->t < closestDist) {
					closestHit = hitrec.value();
					closestDist = hitrec.value().t;
				}
			}
		} else {
			if (stackPtr >= 62) {
				// This should never happen if the BVH is built correctly, but we check it just in case to prevent buffer overflow
				break;
			}
			stack[stackPtr++] = node.offset;
			stack[stackPtr++] = nodeIdx + 1;
		}
	}
	return closestHit;
}

__device__ void Mesh::setMaterial(Material* mat) {
	this->material = mat;
}

__device__ Material* Mesh::getMaterial() const {
	return material;
}