#include "framebuffer.h"

__host__ void Framebuffer::initialize() {
	size_t fbSize = width * height * sizeof(float4);
	if (pixels) checkCudaErrors(cudaFree(pixels));
	checkCudaErrors(cudaMalloc((void**)&pixels, fbSize));
}

__host__ void Framebuffer::cleanup() {
	if (pixels) {
		checkCudaErrors(cudaFree(pixels));
		pixels = nullptr;
	}
}

__device__ void Framebuffer::writePixel(int x, int y, const glm::vec3& color, int frameIndex) {
	int index = y * width + x;
	if (frameIndex <= 1) {
		pixels[index] = make_float4(color.r, color.g, color.b, 1.0f);
	}
	else {
		float4 currentColor = pixels[index];
		pixels[index] = make_float4(currentColor.x + color.r, currentColor.y + color.g, currentColor.z + color.b, 1.0f);
	}
}

__device__ glm::vec3 Framebuffer::color(const Ray& ray, HittableList* world, utils::random::RNG& rng) {
	Ray currentRay = ray;
	glm::vec3 attenuation(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 50; i++) { // depth = 50
		HitScatterRecord HSRec = world->hit(currentRay, 0.001f, INF, rng);
		if (HSRec.hitRec.has_value()) {
			HitRecord hitrec = HSRec.hitRec.value();
			if (HSRec.scatterRec.has_value()) {
				ScatteringRecord scRec = HSRec.scatterRec.value();
				attenuation *= scRec.attenuation;
				currentRay = scRec.ray;
			}
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
	Ray r = camera->getRay(i, j, rng);
	glm::vec3 col = color(r, world, rng);
	return col;
}

__host__ std::shared_ptr<glm::vec3[]> Framebuffer::getHostPixels() const {
	size_t pixelCount = width * height;
	std::unique_ptr<uchar4[]> hostPixelsUchar(new uchar4[pixelCount]);
	checkCudaErrors(cudaMemcpy(hostPixelsUchar.get(), pixels, pixelCount * sizeof(uchar4), cudaMemcpyDeviceToHost));
	std::shared_ptr<glm::vec3[]> hostPixelsFloat(new glm::vec3[pixelCount]);
	for (size_t i = 0; i < pixelCount; i++) {
		uchar4 px = hostPixelsUchar[i];
		hostPixelsFloat[i] = glm::vec3(
			px.x / 255.0f,
			px.y / 255.0f,
			px.z / 255.0f
		);
	}

	return hostPixelsFloat;
}