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

__device__ cu::vec3 color(const Ray& ray) {
	cu::vec3 normalizedDirection = cu::normalize(ray.getDirection());
	float t = 0.5f * (normalizedDirection.y() + 1.0f);
	return (1.0f - t) * cu::vec3(1.0f, 1.0f, 1.0f) + t * cu::vec3(0.5f, 0.7f, 1.0f);
}

__global__ void render(cu::vec3* fb, int x, int y, cu::vec3 bottomLeftCorner, cu::vec3 horizontal, cu::vec3 vertical, cu::vec3 origin) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if ((i >= x) || (j >= y)) return;
    int pixel_index = j * x + i;
    float u = float(i) / float(x);
    float v = float(j) / float(y);
    Ray r(origin, bottomLeftCorner + u * horizontal + v * vertical);
    fb[pixel_index] = color(r);
}

void launchRenderer(cu::vec3* fb, int nx, int ny, int xBlock, int yBlock) {
	int numPixels = nx * ny;
	cu::vec3* devFb = nullptr;
	checkCudaErrors(cudaMalloc((void**)&devFb, numPixels * sizeof(cu::vec3)));

	dim3 blocks(nx / xBlock + 1, ny / yBlock + 1);
	dim3 threads(xBlock, yBlock);
	render<<<blocks, threads>>>(devFb, nx, ny,
		cu::vec3(-2.0f, -1.0f, -1.0f),
		cu::vec3(4.0f, 0.0f, 0.0f),
		cu::vec3(0.0f, 2.0f, 0.0f),
		cu::vec3(0.0f, 0.0f, 0.0f)
		);

	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());
	checkCudaErrors(cudaMemcpy(fb, devFb, numPixels * sizeof(cu::vec3), cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaFree(devFb));
}