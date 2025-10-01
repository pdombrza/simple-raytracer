#include <iostream>
#include <memory>
#include <chrono>
#include <cuda_runtime.h>

#include <glm/glm.hpp>

//#include "camera/camera.h"
//#include "renderer/renderer.h"
#include "hittable/hittable.h"
//#include "hittablelist/hittablelist.h"
//#include "material/material.h"
#include "cudaray/cudaray.h"
#include "window/window.h"
#include "window/windinput.h"

int main() {
	int nx = 1200;
	int ny = 800;
	int xBlock = 16;
	int yBlock = 16;
	std::cerr << "Rendering a " << nx << "x" << ny << " image " << std::endl;
	std::cerr << "in " << xBlock << "x" << yBlock << " blocks" << std::endl;
	int numPixels = nx * ny;

	auto fb = std::make_unique<cu::vec3[]>(numPixels);
	const auto startTime = std::chrono::steady_clock::now();
	launchRenderer(fb.get(), nx, ny, xBlock, yBlock);

	const auto endTime = std::chrono::steady_clock::now();
	const std::chrono::duration<double> renderTime = endTime - startTime;
	std::cout << "Render time: " << renderTime << std::endl;

	auto pxDataGDI = std::make_shared<uint8_t[]>(nx * ny * 4);
	for (int y = 0; y < ny; ++y) {
		for (int x = 0; x < nx; ++x) {
			cu::vec3 color = fb[y * nx + x];
			color = cu::clamp(color, 0.0f, 1.0f);
			int index = (y * nx + x) * 4;
			pxDataGDI[index + 0] = static_cast<uint8_t>(color.b() * 255.0f);
			pxDataGDI[index + 1] = static_cast<uint8_t>(color.g() * 255.0f);
			pxDataGDI[index + 2] = static_cast<uint8_t>(color.r() * 255.0f);
			pxDataGDI[index + 3] = 255;
		}
	}

	Window wind("RT", nx, ny);
	wind.setWindowData(pxDataGDI);
	wind.show();
	wind.processInputLoop();

	return 0;
}
