#include "cudaray.h"

__global__ void renderScene(Framebuffer* d_Fb, Camera* camera, HittableList* world, curandState *randState) {
	int x = d_Fb->getWidth();
	int y = d_Fb->getHeight();
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if ((i >= x) || (j >= y)) return;
    int pixelIdx = j * x + i;
	curandState* localRandState = &randState[pixelIdx];
	utils::random::RNG rng(localRandState);
	d_Fb->writePixel(i, j, d_Fb->colorPixel(i, j, x, y, camera, world, rng));
}

__global__ void initCamera(Camera* cam, int width, int height) {
	if (threadIdx.x == 0 && blockIdx.x == 0) {
		cam->initialize(width, height);
	}
}

__global__ void createWorld(Hittable** d_List, HittableList* d_World) {
	if (threadIdx.x == 0 && blockIdx.x == 0) {
		d_List[0] = new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(glm::vec3(0.5f, 0.5f, 0.5f)));
		d_List[1] = new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(glm::vec3(0.5f, 0.5f, 0.5f)));
		new(d_World) HittableList(d_List, 2, 2);
	}
}

__global__ void destroyWorld(Hittable** d_List, HittableList* d_World, int size) {
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        delete d_List[0]->getMaterial();
        delete d_List[1]->getMaterial();

        delete d_List[0];
        delete d_List[1];

		delete d_World;
    }
}
