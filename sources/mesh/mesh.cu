#include "mesh/mesh.h"


__device__ cuda::std::optional<HitRecord> Mesh::hit(const Ray& ray, float rayTMin, float rayTMax) const {
	cuda::std::optional<HitRecord> closestHit = {};
	float closestDist = rayTMax;
	for (size_t i = 0; i < triangleCount; i++) {
		int idx = i * 3;
		cuda::std::optional<HitRecord> hitrec = rayTriangleIntersection(vertexArray[indexArray[idx]], vertexArray[indexArray[idx + 1]], vertexArray[indexArray[idx + 2]], ray, rayTMin, closestDist);
		if (hitrec.has_value() && hitrec->t < closestDist) {
			closestHit = hitrec.value();
			closestDist = hitrec.value().t;	
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