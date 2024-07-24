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
	
	std::shared_ptr<Lambertian> materialGround = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
	std::shared_ptr<Lambertian> materialCenter = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
	std::shared_ptr<Dielectric> materialLeft = std::make_shared<Dielectric>(1.5f);
	std::shared_ptr<Dielectric> materialBubble = std::make_shared<Dielectric>(1.0f / 1.5f);
	std::shared_ptr<Metal> materialRight = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.0f);

	scene.add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
	scene.add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.2f), 0.5f, materialCenter));
	scene.add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, materialLeft));
	scene.add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.4f, materialBubble));
	scene.add(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, materialRight));

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
