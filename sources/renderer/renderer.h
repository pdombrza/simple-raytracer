#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <sstream>
#include <iostream>

#include "ray.h"
#include "hitrec.h"
#include "hittablelist.h"
#include "material.h"
#include "camera.h"
#include "tobmp.h"
#include "color.h"


class IRenderer {
private:
	virtual glm::vec3 calcRayColor(const Ray& ray, int depth) = 0;
public:
	virtual ~IRenderer() = default;
	virtual int render(Camera& camera) = 0;
	virtual void setScene(HittableList& newScene) = 0;
	virtual HittableList getScene() const = 0;
};


class BMPRenderer : public IRenderer {
private:
	HittableList scene;
	glm::vec3 calcRayColor(const Ray& ray, int depth);
	int imgWidth = 400;
	int samplesPerPixel = 32;
	int maxDepth = 10;
	int imgHeight{};
	float pixelSamplesScale{};
public:
	BMPRenderer(HittableList& scene) : scene(scene) {};
	BMPRenderer(HittableList& scene, int imgWidth, int samplesPerPixel, int maxDepth) : scene(scene), imgWidth(imgWidth), samplesPerPixel(samplesPerPixel), maxDepth(maxDepth) {};
	~BMPRenderer() = default;
	int render(Camera& camera); 
	void setScene(HittableList& newScene) { scene = newScene; };
	HittableList getScene() const { return scene; };
	void setImgWidth(float newImgWidth) { imgWidth = newImgWidth; };
	float getImgWidth() const { return imgWidth; };
	float getImgHeight() const { return imgHeight; };
};