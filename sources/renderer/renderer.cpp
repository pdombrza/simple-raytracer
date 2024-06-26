#include "renderer.h"

glm::vec3 BMPRenderer::calcRayColor(const Ray& ray) {
	std::optional<HitRecord> rec = scene.hit(ray, 0, INF);
	if (rec.has_value()) {
		auto hitColor = 0.5f * (rec.value().normal + glm::vec3(1.0f, 1.0f, 1.0f));
		return hitColor;
	}

	// gradient 
	glm::vec3 direction = ray.getDirection();
	float a = 0.5f * (direction.y + 1.0f);
	return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
}

int BMPRenderer::render(Camera& camera) {
	imgHeight = (int)(imgWidth / camera.getAspectRatio());
	imgHeight = std::max(1, imgHeight);
	pixelSamplesScale = 1.0f / samplesPerPixel;
	camera.initialize(imgWidth, imgHeight);
	std::stringstream stream{};
	stream << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";

	for (size_t i = 0; i < imgHeight; i++) {
		for (size_t j = 0; j < imgWidth; j++) {
			glm::vec3 pxColor(0.0f, 0.0f, 0.0f);
			for (int sample = 0; sample < samplesPerPixel; sample++) {
				Ray r = camera.getRay(j, i);
				pxColor += calcRayColor(r);
			}
			writeColorToStream(stream, pixelSamplesScale * pxColor);
		}
	}

	std::string_view outPath = SOURCE_ROOT "/output.bmp";
	int res = writeBMP(SOURCE_ROOT "/output.bmp", stream);

	if (res < 0) {
		std::cerr << "Failed to create bmp file!" << std::endl;
		return -1;
	}

	return 0;
}