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
	
	auto materialGround = std::make_shared<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f));
	scene.add(std::make_shared<Sphere>(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, materialGround));

	for (int i = -6; i < 6; i++)
	{
		for (int j = -6; j < 6; j++) {
			float chooseMat = Utils::random::getRandomNorm();
			glm::vec3 center(i + 0.9f * Utils::random::getRandomNorm(), 0.2, j + 0.9 * Utils::random::getRandomNorm());
			
			if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
				std::shared_ptr<Material> sphereMaterial;

				if (chooseMat < 0.8) { // Diffuse
					glm::vec3 albedo = Utils::random::randomVec3Norm();
					sphereMaterial = std::make_shared<Lambertian>(albedo);
					scene.add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
				else if (chooseMat < -0.95) { // Metal
					glm::vec3 albedo = Utils::random::randomVec3Norm();
					float fuzz = Utils::random::getRandom(0.0f, 0.5f);
					sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
					scene.add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
				else { // Glass
					sphereMaterial = std::make_shared<Dielectric>(1.5f);
					scene.add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5f);
	scene.add(std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, material1));

	auto material2 = std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f));
	scene.add(std::make_shared<Sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

	auto material3 = std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f);
	scene.add(std::make_shared<Sphere>(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f, material3));


	Camera cam{};
	cam.setVFov(20.0f);
	CameraOrientation orientation{ glm::vec3(13.0f, 2.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) };
	cam.setCameraOrientation(orientation);
	cam.setDefocusAngle(0.6f);
	cam.setFocusDist(10.0f);
	int imgWidth = 800;
	int samplesPerPixel = 256;
	int maxDepth = 50;
	BMPRenderer renderer(scene, imgWidth, samplesPerPixel, maxDepth);

	int result = renderer.render(cam);
	if (result < 0) {
		std::cerr << "It's over" << std::endl;
		return -1;
	}
	return 0;
}
