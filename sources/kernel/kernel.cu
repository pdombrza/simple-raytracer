#include "kernel.h"

__global__ void renderScene(Framebuffer* d_Fb, Camera* camera, HittableList* world, curandState *randState, cudaSurfaceObject_t surfObj) {
	int x = d_Fb->getWidth();
	int y = d_Fb->getHeight();
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if ((i >= x) || (j >= y)) return;
    int pixelIdx = j * x + i;
	curandState* localRandState = &randState[pixelIdx];
	utils::random::RNG rng(localRandState);
	glm::vec3 col = d_Fb->colorPixel(i, j, x, y, camera, world, rng);
	if (surfObj) {
		uchar4 px = make_uchar4(col.r * 255, col.g * 255, col.b * 255, 255);
		surf2Dwrite(px, surfObj, i * sizeof(uchar4), (y - 1 - j));
	}
	else {
		d_Fb->writePixel(i, j, col);
	}
}

__global__ void initCamera(Camera* cam, int width, int height) {
	if (threadIdx.x == 0 && blockIdx.x == 0) {
		cam->initialize(width, height);
	}
}

__global__ void createWorld(Hittable** d_List, HittableList* d_World, curandState *randState) {
	if (threadIdx.x == 0 && blockIdx.x == 0) {
		utils::random::RNG rng(&randState[0]);
		d_List[0] = new Sphere(glm::vec3(0.0f, -1000.0f, -1.0f), 1000.0f, new Lambertian(glm::vec3(0.5f, 0.5f, 0.5f)));
		int count = 1;
		for (int i = -6; i < 6; i++) {
			for (int j = -6; j < 6; j++) {
				float chooseMat = rng.getRandomUniform();
				glm::vec3 center(i + 0.9f * rng.getRandomUniform(), 0.2f, j + 0.9f * rng.getRandomUniform());

				if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {

					if (chooseMat < 0.8f) { // Diffuse
						glm::vec3 albedo = rng.randomVec3Norm();
						d_List[count++] = new Sphere(center, 0.2f, new Lambertian(albedo));
					}
					else if (chooseMat < 0.95f) { // metal
						glm::vec3 albedo = rng.randomVec3Norm();
						float fuzz = rng.getRandom(0.0f, 0.5f);
						d_List[count++] = new Sphere(center, 0.2f, new Metal(albedo, fuzz));
					}
					else { // glass
						d_List[count++] = new Sphere(center, 0.2f, new Dielectric(1.5f));
					}
				}
			}
		}
		d_List[count++] = new Sphere(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, new Dielectric(1.5f));
		d_List[count++] = new Sphere(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(glm::vec3(0.4f, 0.2f, 0.1f)));
		d_List[count++] = new Sphere(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f, new Metal(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f));

		new(d_World) HittableList(d_List, count, 12 * 12 + 1 + 3);
	}
}

__global__ void destroyWorld(Hittable** d_List, HittableList* d_World, int size) {
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        for (int i = 0; i < d_World->getObjCount(); i++) {
			Material* mat = d_List[i]->getMaterial();
			delete mat;
			delete d_List[i];
		}

		delete d_World;
    }
}
