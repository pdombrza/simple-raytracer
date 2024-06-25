#include <iostream>
#include <memory>

#include <glm/glm.hpp>

#include <camera.h>
#include <renderer.h>
#include <hittable.h>
#include <hittablelist.h>

int main() {
	HittableList scene{};

	scene.add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.1f), 100.0f));
	scene.add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
	Camera cam{};
	BMPRenderer renderer(scene);
	int result = renderer.render(cam);
	if (result < 0) {
		std::cerr << "It's over" << std::endl;
		return -1;
	}
	return 0;
}
