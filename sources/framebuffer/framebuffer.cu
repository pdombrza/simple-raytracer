#include "framebuffer.h"


Framebuffer::Framebuffer(Framebuffer&& other) noexcept
	: width(other.width), height(other.height), d_Fb(other.d_Fb) {
	other.d_Fb = nullptr;
	other.width = 0;
	other.height = 0;
}

Framebuffer & Framebuffer::operator=(Framebuffer&& other) noexcept {
	if (this != &other) {
		cleanup();
		width = other.width;
		height = other.height;
		d_Fb = other.d_Fb;
		other.d_Fb = nullptr;
		other.width = 0;
		other.height = 0;
	}
	return *this;
}


__host__ void Framebuffer::initialize() {
	size_t fbSize = width * height * sizeof(glm::vec3);
	if (d_Fb) checkCudaErrors(cudaFree(d_Fb));
	checkCudaErrors(cudaMalloc((void**)&d_Fb, fbSize));
}

__host__ void Framebuffer::cleanup() {
	if (d_Fb) {
		checkCudaErrors(cudaFree(d_Fb));
		d_Fb = nullptr;
	}
}

__device__ void Framebuffer::writePixel(int x, int y, const glm::vec3& color) {
	int index = y * width + x;
	d_Fb[index] = color;
}

__device__ glm::vec3 Framebuffer::color(const Ray& ray, HittableList* world, utils::random::RNG& rng) {
	Ray currentRay = ray;
	float attenuation = 1.0f;
	for (int i = 0; i < 100; i++) { // depth = 50
		HitScatterRecord HSRec = world->hit(currentRay, 0.001f, INF, rng);
		if (HSRec.hitRec.has_value()) {
			HitRecord hitrec = HSRec.hitRec.value();
			glm::vec3 target = hitrec.p + hitrec.normal + rng.randomVec3InSphere();
			attenuation *= 0.5f;
			currentRay = Ray(hitrec.p, target - hitrec.p);
		}
		else {
			glm::vec3 direction = glm::normalize(currentRay.getDirection());
			float a = 0.5f * (direction.y + 1.0f);
			glm::vec3 c = (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
			return attenuation * c;
		}
	}
	return glm::vec3(0.0f, 0.0f, 0.0f); // exceeded recursion depth
}

__device__ glm::vec3 Framebuffer::colorPixel(int i, int j, int nx, int ny, Camera* camera, HittableList* world, utils::random::RNG& rng) {
	glm::vec3 col(0.0f);

	for (int s = 0; s < 100; s++) {
		Ray r = camera->getRay(i, j, rng);
		col += color(r, world, rng);
	}

	col /= float(100);
	col = glm::sqrt(col); // gamma correction
	return col;
}