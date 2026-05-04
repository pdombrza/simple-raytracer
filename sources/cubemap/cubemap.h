#pragma once
#include <vector>
#include <string>

#include <cuda_runtime.h>

#include "utils/utils.h"

class Cubemap {
private:
	cudaArray_t cuArray = nullptr;
	cudaTextureObject_t cuTextureObject{};
public:
	unsigned char* data[6]{};
	int width{};
	int height{};
	int channels{};

	Cubemap() = default;
	Cubemap(const Cubemap&) = delete;
	Cubemap& operator=(const Cubemap&) = delete;
	Cubemap(Cubemap&& other) noexcept;
	Cubemap& operator=(Cubemap&& other) noexcept;
	~Cubemap();
	void loadCubemap(const std::vector<std::string>& faces);
	void allocCubemap();
	void destroyCPUCubemap();
	void destroyGPUBuffers();
	cudaTextureObject_t getTextureObject() const { return cuTextureObject; };
};