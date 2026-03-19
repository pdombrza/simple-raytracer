#include "kernel.h"

__global__ void renderScene(Framebuffer* d_Fb, Camera* camera, HittableList* world, curandState *randState, int frameIndex, cudaSurfaceObject_t surfObj) {
	int x = d_Fb->getWidth();
	int y = d_Fb->getHeight();
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if ((i >= x) || (j >= y)) return;
    int pixelIdx = j * x + i;
	curandState* localRandState = &randState[pixelIdx];
	utils::random::RNG rng(localRandState);
	glm::vec3 col = d_Fb->colorPixel(i, j, x, y, camera, world, rng);
	if (d_Fb->isBad(col)) col = glm::vec3(0.0f);
	d_Fb->writePixel(i, j, col, frameIndex);
	float4 pixelColor = d_Fb->getPixels()[pixelIdx];
	pixelColor.x = sqrtf(pixelColor.x);
	pixelColor.y = sqrtf(pixelColor.y);
	pixelColor.z = sqrtf(pixelColor.z);
	if (surfObj) {
		uchar4 px = make_uchar4(
			static_cast<unsigned char>(glm::clamp(pixelColor.x, 0.0f, 0.999f) * 255.99f), 
			static_cast<unsigned char>(glm::clamp(pixelColor.y, 0.0f, 0.999f) * 255.99f), 
			static_cast<unsigned char>(glm::clamp(pixelColor.z, 0.0f, 0.999f) * 255.99f), 
			255
		);
		surf2Dwrite(px, surfObj, i * sizeof(uchar4), (y - 1 - j));
	}
}

__global__ void initCamera(Camera* cam, int width, int height) {
	if (threadIdx.x == 0 && blockIdx.x == 0) {
		cam->initialize(width, height);
	}
}

__global__ void createWorld(Hittable** d_List, HittableList* d_World) {
	if (threadIdx.x == 0 && blockIdx.x == 0) {
		d_List[0] = new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(glm::vec3(0.1f, 0.2f, 0.5f)));
		d_List[1] = new Sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5, new Metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.5f));
		d_List[2] = new Sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5, new Dielectric(1.5f));
		d_List[3] = new Sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.45, new Dielectric(1.0f / 1.5f));
		d_List[4] = new Triangle(
			glm::vec3(-2.0f, -0.5f, -2.0f),
			glm::vec3(2.0f, -0.5f, -2.0f),
			glm::vec3(0.0f, 1.5f, -2.0f), 
			new Lambertian(glm::vec3(0.4f, 0.2f, 0.1f))
		);
		Material* floor_mat = new Lambertian(glm::vec3(0.8f, 0.8f, 0.0f));
		d_List[5] = new Triangle(
			glm::vec3(-100.0f, -0.5f, 100.0f),
			glm::vec3(-100.0f, -0.5f, -100.0f),
			glm::vec3(100.0f, -0.5f, -100.0f),
			floor_mat
		);

		d_List[6] = new Triangle(
			glm::vec3(-100.0f, -0.5f, 100.0f),
			glm::vec3(100.0f, -0.5f, -100.0f),
			glm::vec3(100.0f, -0.5f, 100.0f),
			floor_mat
		);
		new(d_World) HittableList(d_List, 7, 7);
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
