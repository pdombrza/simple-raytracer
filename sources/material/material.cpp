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
	glm::vec3 reflected = glm::reflect(rayIn.getDirection(), rec.normal) + fuzziness * Utils::random::randomVec3Norm();
	Ray scattered = Ray(rec.p, reflected);
	if (glm::dot(reflected, rec.normal) > 0) {
		ScatteringRecord sRec{};
		sRec.ray = scattered;
		sRec.attenuation = albedo;
		return sRec;
	}
	return {};
}


std::optional<ScatteringRecord> Dielectric::scatter(const Ray& rayIn, const HitRecord& rec) const {
	glm::vec3 attenuation{ 1.0f, 1.0f, 1.0f };
	float reflectionRatio = rec.frontFace ? (1.0f / refractionIndex) : refractionIndex;
	glm::vec3 dir = rayIn.getDirection(); // already unit vector
	glm::vec3 refracted = glm::refract(dir, rec.normal, reflectionRatio);
	Ray scattered = Ray(rec.p, refracted);
	return ScatteringRecord{ scattered, attenuation };
}