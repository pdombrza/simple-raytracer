#include <iostream>
#include <memory>

#include <glm/glm.hpp>

#include <camera.h>
#include <renderer.h>
#include <hittable.h>
#include <hittablelist.h>
#include <material.h>

int main() {
	HittableList scene{};
	
	auto materialGround = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
	auto materialCenter = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
	auto materialRight = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.0f);
	auto materialLeft = std::make_shared<Dielectric>(1.5f);
	auto materialBubble = std::make_shared<Dielectric>(1.0f / 1.5f);

	scene.add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
	scene.add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.2f), 0.5f, materialCenter));
	scene.add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, materialLeft));
	scene.add(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, materialRight));
	scene.add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.4f, materialBubble));

	//scene.sortByDepth(); 

	Camera cam{};
	cam.setVFov(90.0f);
	//CameraOrientation orientation{ glm::vec3(-2.0f, 2.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) };
	//cam.setCameraOrientation(orientation);
	int imgWidth = 400;
	int samplesPerPixel = 100;
	int maxDepth = 50;
	BMPRenderer renderer(scene, imgWidth, samplesPerPixel, maxDepth);

	int result = renderer.render(cam);
	if (result < 0) {
		std::cerr << "It's over" << std::endl;
		return -1;
	}
	return 0;
}
