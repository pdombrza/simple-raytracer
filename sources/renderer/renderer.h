#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <sstream>
#include <iostream>

#include "ray.h"
#include "hitrec.h"
#include "hittablelist.h"
#include "camera.h"
#include "tobmp.h"
#include "color.h"
#include "utils.h"


class IRenderer {
private:
	virtual glm::vec3 calcRayColor(const Ray& ray) = 0;
public:
	virtual ~IRenderer() = default;
	virtual int render(Camera& camera) = 0; // logically this should be const... maybe change structure later?
	virtual void setScene(HittableList& newScene) = 0;
	virtual HittableList getScene() const = 0;
};


class BMPRenderer : public IRenderer {
private:
	HittableList scene;
	glm::vec3 calcRayColor(const Ray& ray);
	int imgWidth = 400;
	int imgHeight{};
public:
	BMPRenderer(HittableList& scene) : scene(scene) {};
	BMPRenderer(HittableList& scene, int imgWidth) : scene(scene), imgWidth(imgWidth) {};
	~BMPRenderer() = default;
	int render(Camera& camera);  // ... same thing
	void setScene(HittableList& newScene) { scene = newScene; };
	HittableList getScene() const { return scene; };
	void setImgWidth(float newImgWidth) { imgWidth = newImgWidth; };
	float getImgWidth() const { return imgWidth; };
	float getImgHeight() const { return imgHeight; };
};