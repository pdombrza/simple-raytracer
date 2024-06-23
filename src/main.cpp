#include <iostream>
#include <sstream>
#include <fstream>

#include <glm/glm.hpp>

#include <color.h>
#include <tobmp.h>

int main() {
	const int imgWidth = 256;
	const int imgHeight = 256;

	// ppm header
	std::stringstream stream;
	//std::ofstream ppmFile(SOURCE_ROOT "/img.ppm");
	stream << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";

	for (size_t i = 0; i < imgHeight; i++) {
		for (size_t j = 0; j < imgWidth; j++) {
			glm::vec3 pixelColor = glm::vec3((float)i/(imgWidth-1), (float)j/(imgHeight-1), 0.f);
			writeColorToStream(stream, pixelColor);
		}
	}

	std::string outPath = SOURCE_ROOT "/output.bmp";
	int res = writeBMP(SOURCE_ROOT "/output.bmp", stream);

	if (res<0) {
		std::cerr << "Failed to create bmp file!" << std::endl;
		return -1;
	}

	return 0;
}
