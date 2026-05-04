#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "cubemap.h"

Cubemap::Cubemap(Cubemap&& other) noexcept {
	cuArray = nullptr;
	cuTextureObject = 0;
	for (int i = 0; i < 6; i++) data[i] = nullptr;
	*this = std::move(other);
}

Cubemap& Cubemap::operator=(Cubemap&& other) noexcept {
	if (this != &other) {
		destroyGPUBuffers();
		destroyCPUCubemap();
		cuArray = other.cuArray;
		cuTextureObject = other.cuTextureObject;
		width = other.width;
		height = other.height;
		channels = other.channels;
		for (int i = 0; i < 6; i++) {
			data[i] = other.data[i];
		}
		other.cuArray = nullptr;
		other.cuTextureObject = 0;
		for (int i = 0; i < 6; i++) {
			other.data[i] = nullptr;
		}
	}
	return *this;
}

Cubemap::~Cubemap() {
	destroyGPUBuffers();
	destroyCPUCubemap();
}

void Cubemap::loadCubemap(const std::vector<std::string>& faces) {
	for (size_t i = 0; i < faces.size(); i++) {
		data[i] = stbi_load(faces[i].c_str(), &width, &height, &channels, 4);
		if (!data[i]) {
			throw std::runtime_error("Failed to load cubemap texture: " + faces[i]);
		}
	}
}

void Cubemap::destroyCPUCubemap() {
	for (size_t i = 0; i < 6; i++) {
		if (data[i]) {
			stbi_image_free(data[i]);
			data[i] = nullptr;
		}
	}
}

void Cubemap::allocCubemap() {
	cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc<uchar4>();
	cudaExtent extent = make_cudaExtent(width, height, 6);
	checkCudaErrors(cudaMalloc3DArray(&cuArray, &channelDesc, extent, cudaArrayCubemap));
	

	for (int i = 0; i < 6; i++) {
		cudaMemcpy3DParms copyParams = { 0 };
		copyParams.srcPtr = make_cudaPitchedPtr(data[i], width * 4, width, height);
		copyParams.dstArray = cuArray;
		copyParams.dstPos = make_cudaPos(0, 0, i);
		copyParams.extent = make_cudaExtent(width, height, 1);
		copyParams.kind = cudaMemcpyHostToDevice;
		checkCudaErrors(cudaMemcpy3D(&copyParams));
	}
	cudaResourceDesc resDesc = {};
	resDesc.resType = cudaResourceTypeArray;
	resDesc.res.array.array = cuArray;

	cudaTextureDesc texDesc = {};
	texDesc.addressMode[0] = cudaAddressModeClamp;
	texDesc.addressMode[1] = cudaAddressModeClamp;
	texDesc.addressMode[2] = cudaAddressModeClamp;
	texDesc.filterMode = cudaFilterModeLinear;

	texDesc.readMode = cudaReadModeNormalizedFloat;
	checkCudaErrors(cudaCreateTextureObject(&cuTextureObject, &resDesc, &texDesc, nullptr));
	destroyCPUCubemap();
}

void Cubemap::destroyGPUBuffers() {
	if (cuTextureObject) {
		checkCudaErrors(cudaDestroyTextureObject(cuTextureObject));
		cuTextureObject = 0;
	}
	if (cuArray) {
		checkCudaErrors(cudaFreeArray(cuArray));
		cuArray = nullptr;
	}
}