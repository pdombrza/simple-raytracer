#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string_view>
#include <optional>
#include <memory>

#include <glm/glm.hpp>

#include <color.h>
#include <ray.h>
#include <tobmp.h>
#include <hittable.h>
#include <hitrec.h>
#include <hittablelist.h>

#define INF std::numeric_limits<float>::max()
   
glm::vec3 rayColor(const Ray& ray, const HittableList& world) {
	std::optional<HitRecord> rec = world.hit(ray, 0, INF);
	if (rec.has_value()) {
		auto hitColor = 0.5f * (rec.value().normal + glm::vec3(1.0f, 1.0f, 1.0f));
		return hitColor;
	}

	// gradient 
	glm::vec3 direction = ray.getDirection();
	float a = 0.5f * (direction.y + 1.0f);
	return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
}

int main() {
	float aspectRatio = 16.0f / 9.0f;
	int imgWidth = 400;

	int imgHeight = (int) (imgWidth / aspectRatio);
	imgHeight = std::max(1, imgHeight);

	HittableList scene{};

	scene.add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.1f), 100.0f));
	scene.add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));


	float focalLength = 1.0f;
	float viewportHeight = 2.0f;
	float viewportWidth = viewportHeight * ((float)imgWidth / imgHeight);
	glm::vec3 cameraCenter( 0.0f, 0.0f, 0.0f );

	glm::vec3 viewportU(viewportWidth, 0.0f, 0.0f );
	glm::vec3 viewportV( 0.0f, -viewportHeight, 0.0f );

	glm::vec3 pixelDeltaU = viewportU / (float)imgWidth;
	glm::vec3 pixelDeltaV = viewportV / (float)imgHeight;

	glm::vec3 viewportUpperLeft = cameraCenter - glm::vec3(0.0f, 0.0f, focalLength) - viewportU / 2.0f - viewportV / 2.0f;
	glm::vec3 startPixelLoc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);
		

	std::stringstream stream;
	stream << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";

	for (size_t i = 0; i < imgHeight; i++) {
		for (size_t j = 0; j < imgWidth; j++) {
			glm::vec3 pixelCenter = startPixelLoc + ((float)j * pixelDeltaU) + ((float)i * pixelDeltaV);
			glm::vec3 rayDirection = pixelCenter - cameraCenter;
			Ray r(cameraCenter, rayDirection);
			glm::vec3 pxColor = rayColor(r, scene);
			writeColorToStream(stream, pxColor);
		}
	}

	std::string_view outPath = SOURCE_ROOT "/output.bmp";
	int res = writeBMP(SOURCE_ROOT "/output.bmp", stream);

	if (res<0) {
		std::cerr << "Failed to create bmp file!" << std::endl;
		return -1;
	}

	return 0;
}
