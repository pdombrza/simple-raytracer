#include "renderer.h"


void CudaRenderer::initRenderer() {
	int numPixels = imgWidth * imgHeight;
	checkCudaErrors(cudaMalloc((void**)&d_Fb, sizeof(Framebuffer)));
	checkCudaErrors(cudaMemcpy(d_Fb, &h_Fb, sizeof(Framebuffer), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMalloc((void**)&d_camera, sizeof(Camera)));
	checkCudaErrors(cudaMalloc((void**)&d_List, 5 * sizeof(Hittable*)));
	checkCudaErrors(cudaMalloc((void**)&d_World, sizeof(HittableList)));
	checkCudaErrors(cudaMalloc((void**)&d_randStates, numPixels * sizeof(curandState)));
}

CudaRenderer::~CudaRenderer() {
	checkCudaErrors(cudaFree(d_List));
	d_List = nullptr;
	checkCudaErrors(cudaFree(d_World));
	d_World = nullptr;
	checkCudaErrors(cudaFree(d_Fb));
	d_Fb = nullptr;
	checkCudaErrors(cudaFree(d_camera));
	d_camera = nullptr;
	checkCudaErrors(cudaFree(d_randStates));
	d_randStates = nullptr;
}

int CudaRenderer::render(Camera& camera) {
	int numPixels = imgWidth * imgHeight;
	
	checkCudaErrors(cudaMemcpy(d_camera, &camera, sizeof(Camera), cudaMemcpyHostToDevice));
	initCamera<<<1, 1>>>(d_camera, imgWidth, imgHeight);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());

	createWorld<<<1, 1>>>(d_List, d_World);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());

	dim3 blocks(imgWidth / xBlock + 1, imgHeight / yBlock + 1);
	dim3 threads(xBlock, yBlock);
	utils::random::randomInit<<<blocks, threads>>>(d_randStates, imgWidth, imgHeight);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());
	renderScene<<<blocks, threads>>>(d_Fb, d_camera, d_World, d_randStates);

	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());
	destroyWorld<<<1, 1>>>(d_List, d_World, 2);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaDeviceSynchronize());
	return 0;
}