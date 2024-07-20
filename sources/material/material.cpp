#include "material.h"


std::optional<ScatteringRecord> Lambertian::scatter(const Ray& rayIn, const HitRecord& rec) const {
	glm::vec3 scatterDirection = rec.normal + Utils::random::randomVec3Norm();

	// Catch degenerate scatter direction
	if (glm::all(glm::epsilonEqual(scatterDirection, glm::vec3(0.0f, 0.0f, 0.0f), std::numeric_limits<float>::epsilon())))
		scatterDirection = rec.normal;

	Ray scattered = Ray(rec.p, scatterDirection);
	return ScatteringRecord{scattered, albedo};
}


std::optional<ScatteringRecord> Metal::scatter(const Ray& rayIn, const HitRecord& rec) const {
	glm::vec3 reflected = glm::reflect(rayIn.getDirection(), rec.normal);
	Ray scattered = Ray(rec.p, reflected);
	ScatteringRecord sRec{};
	sRec.ray = scattered;
	sRec.attenuation = albedo;
	return sRec;
}