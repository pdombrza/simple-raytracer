#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <execution>

#include "ray/ray.h"
#include "hitrec/hitrec.h"
#include "hittablelist/hittablelist.h"
#include "material/material.h"
#include "camera/camera.h"
#include "tobmp/tobmp.h"
#include "color/color.h"


class IRenderer {
protected:
	virtual glm::vec3 calcRayColor(const Ray& ray, int depth) = 0;
public:
	virtual ~IRenderer() = default;
	virtual int render(Camera& camera) = 0;
	virtual void setScene(HittableList& newScene) = 0;
	virtual HittableList getScene() const = 0;
};


class BMPRenderer : public IRenderer {
protected:
	virtual glm::vec3 calcRayColor(const Ray& ray, int depth) override;
	HittableList scene;
	int imgWidth = 400;
	int samplesPerPixel = 32;
	int maxDepth = 10;
	int imgHeight{};
	float pixelSamplesScale{};
public:
	BMPRenderer(HittableList& scene) : scene(scene) {};
	BMPRenderer(HittableList& scene, int imgWidth, int samplesPerPixel, int maxDepth) : scene(scene), imgWidth(imgWidth), samplesPerPixel(samplesPerPixel), maxDepth(maxDepth) {};
	~BMPRenderer() = default;
	virtual int render(Camera& camera) override; 
	virtual void setScene(HittableList& newScene) override { scene = newScene; };
	virtual HittableList getScene() const override { return scene; } ;
	virtual void setImgWidth(float newImgWidth) { imgWidth = newImgWidth; };
	virtual float getImgWidth() const { return imgWidth; };
	virtual float getImgHeight() const { return imgHeight; };
};


class MT_BMPRenderer : public BMPRenderer {
private:
	std::vector<int> widthVecIter, heightVecIter;
protected:
	std::unique_ptr<glm::vec3[]> pxBuffer = nullptr;
public:
	using BMPRenderer::BMPRenderer;
	virtual void setPxBuffer(std::unique_ptr<glm::vec3[]> pxbuf) { pxBuffer = std::move(pxbuf); };
	virtual void populatePxBuffer(Camera& camera);
	virtual int render(Camera& camera) override;
};


class MT_WindowRenderer : public MT_BMPRenderer {
private:
	std::vector<int> widthVecIter, heightVecIter;
protected:
	std::shared_ptr<uint8_t[]> rgbBuffer = nullptr;
public:
	virtual void pxBufToGDI(int imgWidth, int imgHeight);
	using MT_BMPRenderer::MT_BMPRenderer;
	virtual int render(Camera& camera) override;
	std::shared_ptr<uint8_t[]> getRgbBuffer() { return rgbBuffer; };
};