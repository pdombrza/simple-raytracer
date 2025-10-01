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

__device__ cu::vec3 color(const Ray& ray, HittableList* world) {
	HitScatterRecord HSRec = world->hit(ray, 0.001f, INF);
	if (HSRec.hitRec.has_value()) {
		HitRecord hitrec = HSRec.hitRec.value();
		return 0.5f * (hitrec.normal + cu::vec3(1.0f, 1.0f, 1.0f));
	}

	// gradient
	cu::vec3 direction = ray.getDirection();
	float a = 0.5f * (direction.y() + 1.0f);
	return (1.0f - a) * cu::vec3(1.0f, 1.0f, 1.0f) + a * cu::vec3(0.5f, 0.7f, 1.0f);
}

__global__ void render(cu::vec3* fb, int x, int y, cu::vec3 bottomLeftCorner, cu::vec3 horizontal, cu::vec3 vertical, cu::vec3 origin, HittableList* world) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if ((i >= x) || (j >= y)) return;
    int pixel_index = j * x + i;
    float u = float(i) / float(x);
    float v = float(j) / float(y);
    Ray r(origin, bottomLeftCorner + u * horizontal + v * vertical);
    fb[pixel_index] = color(r, world);
}

void launchRenderer(cu::vec3* fb, int nx, int ny, int xBlock, int yBlock) {
	int numPixels = nx * ny;
	cu::vec3* devFb = nullptr;
	checkCudaErrors(cudaMalloc((void**)&devFb, numPixels * sizeof(cu::vec3)));
	Hittable** d_List;
	checkCudaErrors(cudaMalloc((void**)&d_List, 2 * sizeof(Hittable*)));
	HittableList* d_World;
	checkCudaErrors(cudaMalloc((void**)&d_World, sizeof(HittableList)));
	createWorld<<<1, 1>>>(d_List, d_World);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());

	dim3 blocks(nx / xBlock + 1, ny / yBlock + 1);
	dim3 threads(xBlock, yBlock);
	render<<<blocks, threads>>>(devFb, nx, ny,
		cu::vec3(-2.0f, -1.0f, -1.0f),
		cu::vec3(4.0f, 0.0f, 0.0f),
		cu::vec3(0.0f, 2.0f, 0.0f),
		cu::vec3(0.0f, 0.0f, 0.0f),
		d_World	
		);

	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());
	destroyWorld<<<1, 1>>>(d_List, d_World);
	checkCudaErrors(cudaGetLastError());

	checkCudaErrors(cudaFree(d_List));
	checkCudaErrors(cudaFree(d_World));
	checkCudaErrors(cudaMemcpy(fb, devFb, numPixels * sizeof(cu::vec3), cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaFree(devFb));
	cudaDeviceReset();
}

__global__ void createWorld(Hittable** d_List, HittableList* d_World) {
	if (threadIdx.x == 0 && blockIdx.x == 0) {
		d_List[0] = new Sphere(cu::vec3(0.0f, 0.0f, -1.0f), 0.5f);
		d_List[1] = new Sphere(cu::vec3(0.0f, -100.5f, -1.0f), 100.0f);
		new(d_World) HittableList(d_List, 2);
	}
}

__global__ void destroyWorld(Hittable ** d_List, HittableList* d_World) {
	delete d_List[0];
	delete d_List[1];
	delete d_World;
}