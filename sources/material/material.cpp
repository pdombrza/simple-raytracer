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
	const float reflectionRatio = rec.frontFace ? (1.0f / refractionIndex) : refractionIndex;

	glm::vec3 dir = rayIn.getDirection(); // already unit vector
	const float cosTheta = std::min(glm::dot(-dir, rec.normal), 1.0f);
	float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

	bool cannotRefract = reflectionRatio * sinTheta > 1.0f;

	glm::vec3 scatteredDirection{};
	
	if (cannotRefract || reflectance(cosTheta, reflectionRatio) > Utils::random::getRandomNorm()) {
		scatteredDirection = glm::reflect(dir, rec.normal);
	}
	else
		scatteredDirection = glm::refract(dir, rec.normal, reflectionRatio);

	Ray scattered = Ray(rec.p, scatteredDirection);
	return ScatteringRecord{ scattered, attenuation };
}


float Dielectric::reflectance(const float cosine, const float refractionIndex) {
	// Schlick's approximation for reflectance
	float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5);
}