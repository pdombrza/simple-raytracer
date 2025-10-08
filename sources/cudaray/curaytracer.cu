#include "cudaray.h"

void checkCuda(cudaError_t result, char const* const func, const char* const file, int const line) {
    if (result) {
        std::cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
            file << ":" << line << " '" << func << "' \n";
        // Make sure we call CUDA Device Reset before exiting
        cudaDeviceReset();	
        exit(99);
    }
}

__device__ glm::vec3 color(const Ray& ray, HittableList* world, utils::random::RNG& rng) {
	Ray currentRay = ray;
	float attenuation = 1.0f;
	for (int i = 0; i < 100; i++) { // depth = 50
		HitScatterRecord HSRec = world->hit(currentRay, 0.001f, INF, rng);
		if (HSRec.hitRec.has_value()) {
			HitRecord hitrec = HSRec.hitRec.value();
			glm::vec3 target = hitrec.p + hitrec.normal + rng.randomVec3InSphere();
			attenuation *= 0.5f;
			currentRay = Ray(hitrec.p, target - hitrec.p);
		}
		else {
			glm::vec3 direction = glm::normalize(currentRay.getDirection());
			float a = 0.5f * (direction.y + 1.0f);
			glm::vec3 c = (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
			return attenuation * c;
		}
	}
	return glm::vec3(0.0f, 0.0f, 0.0f); // exceeded recursion depth
}

__device__ glm::vec3 colorPixel(int i, int j, int nx, int ny, Camera* camera, HittableList* world, utils::random::RNG& rng) {
	glm::vec3 col(0.0f);

	for (int s = 0; s < 100; s++) {
		Ray r = camera->getRay(i, j, rng);
		col += color(r, world, rng);
	}

	col /= float(100);
	col = glm::sqrt(col); // gamma correction
	return col;
}


__global__ void render(glm::vec3* fb, int x, int y, Camera* camera, HittableList* world, curandState *randState) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if ((i >= x) || (j >= y)) return;
    int pixelIdx = j * x + i;
	curandState localRandState = randState[pixelIdx];
	utils::random::RNG rng(&localRandState);
	fb[pixelIdx] = colorPixel(i, j, x, y, camera, world, rng);
}

void launchRenderer(glm::vec3* fb, int nx, int ny, int xBlock, int yBlock) {
	int numPixels = nx * ny;
	CameraOrientation orientation;
	orientation.lookFrom = glm::vec3(0.0f, 0.0f, 1.0f);
	orientation.lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	orientation.vUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Camera h_camera(orientation, 90.0f, (float)nx / (float)ny);
	Camera* d_camera;
	checkCudaErrors(cudaMalloc((void**)&d_camera, sizeof(Camera)));
	checkCudaErrors(cudaMemcpy(d_camera, &h_camera, sizeof(Camera), cudaMemcpyHostToDevice));
	initCamera<<<1, 1>>>(d_camera, nx, ny);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());

	glm::vec3* d_Fb = nullptr;
	checkCudaErrors(cudaMalloc((void**)&d_Fb, numPixels * sizeof(glm::vec3)));

	Hittable** d_List;
	checkCudaErrors(cudaMalloc((void**)&d_List, 2 * sizeof(Hittable*)));
	HittableList* d_World;
	checkCudaErrors(cudaMalloc((void**)&d_World, sizeof(HittableList)));
	createWorld<<<1, 1>>>(d_List, d_World);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());

	curandState* d_randStates;
	checkCudaErrors(cudaMalloc((void**)&d_randStates, numPixels * sizeof(curandState)));

	dim3 blocks(nx / xBlock + 1, ny / yBlock + 1);
	dim3 threads(xBlock, yBlock);
	utils::random::randomInit<<<blocks, threads>>>(d_randStates, nx, ny);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());
	render<<<blocks, threads>>>(d_Fb, nx, ny, d_camera, d_World, d_randStates);

	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());
	destroyWorld<<<1, 1>>>(d_List, d_World);
	checkCudaErrors(cudaGetLastError());

	//checkCudaErrors(cudaFree(d_List));
	//checkCudaErrors(cudaFree(d_World));
	checkCudaErrors(cudaMemcpy(fb, d_Fb, numPixels * sizeof(glm::vec3), cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaFree(d_Fb));
	checkCudaErrors(cudaFree(d_randStates));
	checkCudaErrors(cudaFree(d_camera));
	cudaDeviceReset();
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

__global__ void destroyWorld(Hittable** d_List, HittableList* d_World) {
	int capacity = d_World->getCapacity();
	for (int i = 0; i < capacity; i++) {
		delete d_List[i];
	}
	delete d_World;
}
