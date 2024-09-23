#include "renderer.h"

glm::vec3 BMPRenderer::calcRayColor(const Ray& ray, int depth) {
	if (depth < 0)
		return glm::vec3(0.0f, 0.0f, 0.0f); // if exceeds maxdepth do not generate more light
	HitScatterRecord HSRec = scene.hit(ray, 0.001f, INF);
	if (HSRec.hitRec.has_value()) {
		// return a separate struct which will return HitRecord and ScatteringRecord (now hittableobjectlist.hit returns HitRecord without material info)
		HitRecord hitrec = HSRec.hitRec.value();
		if (HSRec.scatterRec.has_value()) {
			ScatteringRecord scRec = HSRec.scatterRec.value();
			return scRec.attenuation * calcRayColor(scRec.ray, depth - 1);
		}
		return glm::vec3(0.0f, 0.0f, 0.0f); // return color(0,0,0)
	}

	// gradient 
	glm::vec3 direction = ray.getDirection();
	float a = 0.5f * (direction.y + 1.0f);
	return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
}

int BMPRenderer::render(Camera& camera) {
	imgHeight = (int)(imgWidth / camera.getAspectRatio());
	imgHeight = std::max(1, imgHeight);
	pixelSamplesScale = 1.0f / samplesPerPixel;
	camera.initialize(imgWidth, imgHeight);
	std::stringstream stream{};
	stream << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";

	for (size_t i = 0; i < imgHeight; i++) {
		for (size_t j = 0; j < imgWidth; j++) {
			glm::vec3 pxColor(0.0f, 0.0f, 0.0f);
			for (int sample = 0; sample < samplesPerPixel; sample++) {
				Ray r = camera.getRay(j, i);
				pxColor += calcRayColor(r, maxDepth);
			}
			writeColorToStream(stream, pixelSamplesScale * pxColor);
		}
	}

	int res = writeBMP(SOURCE_ROOT "/output.bmp", stream);

	if (res < 0) {
		std::cerr << "Failed to create bmp file!" << std::endl;
		return -1;
	}

	return 0;
}


int MT_BMPRenderer::render(Camera& camera) {
	imgHeight = std::max(1, (int)(imgWidth / camera.getAspectRatio()));
	pixelSamplesScale = 1.0f / samplesPerPixel;
	camera.initialize(imgWidth, imgHeight);

	// We can get away with resizing in render function since it's called only once
	//pxBuffer = new glm::vec3[imgWidth * imgHeight];
	//memset(pxBuffer, 0, imgWidth * imgHeight * sizeof(glm::vec3));
	pxBuffer = std::make_unique<glm::vec3[]>(imgWidth * imgHeight);
	widthVecIter.resize(imgWidth); 
	heightVecIter.resize(imgHeight); 
	for (size_t i = 0; i < imgWidth; i++) {
		widthVecIter[i] = i;
	}
	for (size_t i = 0; i < imgHeight; i++) {
		heightVecIter[i] = i;
	}

	std::for_each(std::execution::par, heightVecIter.begin(), heightVecIter.end(),
		[this, &camera](size_t i) {
			std::for_each(std::execution::par, widthVecIter.begin(), widthVecIter.end(),
				[this, &camera, i](size_t j) {
					glm::vec3 pxColor(0.0f, 0.0f, 0.0f);
					for (int sample = 0; sample < samplesPerPixel; sample++) {
						Ray r = camera.getRay(j, i);
						pxColor += calcRayColor(r, maxDepth);
					}
					pxBuffer[i * imgWidth + j] = pxColor * pixelSamplesScale;
				});
		});

	//for (size_t i = 0; i < imgHeight; i++) {
	//	for (size_t j = 0; j < imgWidth; j++) {
	//		glm::vec3 pxColor(0.0f, 0.0f, 0.0f);
	//		for (int sample = 0; sample < samplesPerPixel; sample++) {
	//			Ray r = camera.getRay(j, i);
	//			pxColor += calcRayColor(r, maxDepth);
	//		}
	//		pxBuffer[i * imgWidth + j] = pxColor * pixelSamplesScale;
	//	}
	//}

	std::stringstream imgStream = writeBufferToStream(std::move(pxBuffer), imgWidth, imgHeight);

	int res = writeBMP(SOURCE_ROOT "/output.bmp", imgStream);

	if (res < 0) {
		std::cerr << "Failed to crete bmp file!" << std::endl;
		return -1;
	}

	return 0;
}
