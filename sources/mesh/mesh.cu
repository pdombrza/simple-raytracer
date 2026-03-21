#include "mesh/mesh.h"


__device__ Mesh::~Mesh() {
	if (triangles) {
		for (size_t i = 0; i < triangleCount; ++i) {
			delete triangles[i];
		}
		delete[] triangles;
		triangles = nullptr;
	}
	if (material) {
		delete material;
		material = nullptr;
	}
};

__device__ cuda::std::optional<HitRecord> Mesh::hit(const Ray& ray, float rayTMin, float rayTMax) const {
	cuda::std::optional<HitRecord> closestHit = {};
	float closestDist = rayTMax;
	for (size_t i = 0; i < triangleCount; i++) {
		cuda::std::optional<HitRecord> hitrec = triangles[i]->hit(ray, rayTMin, rayTMax);
		if (hitrec.has_value() && hitrec->t < closestDist) {
			closestHit = hitrec.value();
			closestDist = hitrec.value().t;	
		}
	}
	return closestHit;
}

__device__ void Mesh::setMaterial(Material* mat) {
	this->material = mat;
	for (size_t i = 0; i < triangleCount; i++) {
		triangles[i]->setMaterial(mat);
	}
}

__device__ Material* Mesh::getMaterial() const {
	return material;
}

__device__ glm::vec3 Mesh::getCenter() const {
	glm::vec3 center{ 0.0f, 0.0f, 0.0f };
	for (size_t i = 0; i < triangleCount; i++) {
		center += triangles[i]->getCenter();
	}
	return center / static_cast<float>(triangleCount);
}

__device__ HitRecord Mesh::constructHitRecord(const Ray& ray, float t) const {
	HitRecord hitrec = {};
	hitrec.t = t;
	hitrec.p = ray.At(t);
	hitrec.normal = glm::normalize(hitrec.p - center);
	hitrec.setFaceNormal(ray, hitrec.normal);
	return hitrec;
}