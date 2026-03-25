#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <fstream>

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

#include "ray/ray.h"
#include "hitrec/hitrec.h"
#include "hittable/hittable.h"
#include "hittablelist/hittablelist.h"
#include "material/material.h"
#include "camera/camera.h"
#include "tobmp/tobmp.h"
#include "color/color.h"
#include "kernel/kernel.h"


class IRenderer {
public:
	virtual ~IRenderer() = default;
	virtual int render(Camera& camera, bool resetFrameIndex) = 0;
	virtual void setScene(HittableList* newScene) = 0;
	virtual HittableList* getScene() const = 0;
};

class CudaRenderer : public IRenderer {
private:
	Framebuffer h_Fb;
	Framebuffer* d_Fb = nullptr;
	Camera* d_camera = nullptr;
	Hittable** d_List = nullptr;
	HittableList* d_World = nullptr;
	curandState* d_randStates = nullptr;
	glm::vec3* d_vertices = nullptr;
	Mesh* d_meshes = nullptr;
	int* d_indices = nullptr;
	std::vector<MeshDescriptor> meshDescriptors;
	std::vector<glm::vec3> h_vertices;
	std::vector<int> h_indices;
	cudaGraphicsResource* glResource = nullptr;
protected:
	HittableList* scene;
	int imgWidth = 400;
	int imgHeight = 225;
	int samplesPerPixel = 32;
	int maxDepth = 10;
	int xBlock = 16;
	int yBlock = 16;
	int frameIndex = 1;
	int numMeshes = 0;
	int numObjects = 0;
	float pixelSamplesScale{};
	virtual void initRenderer();
public:
	CudaRenderer(HittableList* scene, std::vector<MeshDescriptor> meshDescriptors, std::vector<glm::vec3> vertexArray, std::vector<int> indexArray) : scene(scene), meshDescriptors(meshDescriptors), h_vertices(vertexArray), h_indices(indexArray), h_Fb(imgWidth, imgHeight) { initRenderer(); };
	CudaRenderer(HittableList* scene, int imgWidth, int imgHeight, int samplesPerPixel, int maxDepth) : scene(scene), imgWidth(imgWidth), imgHeight(imgHeight), samplesPerPixel(samplesPerPixel), maxDepth(maxDepth), h_Fb(imgWidth, imgHeight) { initRenderer(); };
	CudaRenderer(HittableList* scene, int imgWidth, int imgHeight) : scene(scene), imgWidth(imgWidth), imgHeight(imgHeight), h_Fb(imgWidth, imgHeight) { initRenderer(); };
	~CudaRenderer();
	virtual void setXBlock(int newXBlock) { xBlock = newXBlock; };
	virtual int getXBlock() const { return xBlock; };
	virtual void setYBlock(int newYBlock) { yBlock = newYBlock; };
	virtual int getYBlock() const { return yBlock; };
	virtual void setScene(HittableList* newScene) override { scene = newScene; };
	virtual HittableList* getScene() const override { return scene; };
	virtual void setImgWidth(int newImgWidth) { imgWidth = newImgWidth; };
	virtual int getImgWidth() const { return imgWidth; };
	virtual void setImgHeight(int newImgHeight) { imgHeight = newImgHeight; };
	virtual int getImgHeight() const { return imgHeight; };
	virtual void setNumMeshes(int newNumMeshes) { numMeshes = newNumMeshes; };
	virtual int getNumMeshes() const { return numMeshes; };
	virtual void setNumObjects(int newNumObjects) { numObjects = newNumObjects; };
	virtual int getNumObjects() const { return numObjects; };
	virtual void setHostMeshes(std::vector<MeshDescriptor> newMeshes) { meshDescriptors = newMeshes; };
	virtual void setMeshData(std::vector<glm::vec3> vertexArray, std::vector<int> indexArray, std::vector<MeshDescriptor> descriptors);
	void registerGLTexture(GLuint glTex);
	virtual void setupScene(Camera& camera);
	virtual void destroyScene() const;
	virtual int render(Camera& camera, bool resetFrameIndex) override;
	virtual std::vector<MeshDescriptor> getHostMeshes() const { return meshDescriptors; };
	std::shared_ptr<glm::vec3[]> getHostPixels() const { return h_Fb.getHostPixels(); };
};