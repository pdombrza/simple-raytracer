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

__global__ void createWorld(Hittable** d_List, HittableList* d_World, glm::vec3* vertexArray, int* indexArray, MeshDescriptor* descriptors, int meshCount, Mesh* d_meshes, int objCount) {
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        d_List[0] = new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(glm::vec3(0.1f, 0.2f, 0.5f)));
        d_List[1] = new Sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.5f));
        d_List[2] = new Sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, new Dielectric(1.5f));
        d_List[3] = new Sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.45f, new Dielectric(1.0f / 1.5f));
        Material* floorMaterial = new Lambertian(glm::vec3(0.8f, 0.8f, 0.0f));
        d_List[4] = new Triangle(glm::vec3(-100.f, -0.5f, 100.f), glm::vec3(-100.f, -0.5f, -100.f), glm::vec3(100.f, -0.5f, -100.f), floorMaterial);
        d_List[5] = new Triangle(glm::vec3(-100.f, -0.5f, 100.f), glm::vec3(100.f, -0.5f, -100.f), glm::vec3(100.f, -0.5f, 100.f), floorMaterial);

        for (int i = 0; i < meshCount; i++) {
			MeshDescriptor& desc = descriptors[i];
			glm::vec3* meshVertices = vertexArray + desc.vertexOffset;
			int* meshIndices = indexArray + desc.indexOffset;
			Material* mat = new Lambertian(glm::vec3(0.1f, 0.8f, 0.1f)); 
			d_meshes[i] = Mesh(meshVertices, meshIndices, desc.triangleCount, mat);
		}
        new(d_World) HittableList(d_List, objCount, d_meshes, meshCount, objCount);
    }
}

__global__ void destroyWorld(Hittable** d_List, HittableList* d_World, Mesh* d_meshes) {
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        for (int i = 0; i < d_World->getObjCount(); i++) {
			delete d_List[i]->getMaterial();;
			delete d_List[i];
		}

		for (int i = 0; i < d_World->getMeshCount(); i++) {
			delete d_meshes[i].getMaterial();
		}
		delete d_World;
    }
}
