#pragma once

#include <stdint.h>
#include <sstream>
#include <string>


int writeBMP(const std::string& filename, std::stringstream& ppmData); // c style function baby

#pragma pack(push, 1)
struct BMPHeader {
	uint16_t bfType{ 0x4D42 };
	uint32_t bfSize{ 0 };
	uint16_t bfReserved1{ 0 };
	uint16_t bfReserved2{ 0 };
	uint32_t bfOffBits{ 54 }; // 14 bytes for header and 40 for info header
};

struct BMPInfoHeader {
	uint32_t biSize{ 40 };
	int32_t biWidth{ 0 };
	int32_t biHeight{ 0 };
	uint16_t biPlanes{ 1 };
	uint16_t biBitCount{ 24 };
	uint32_t biCompression{ 0 }; // no compression
	uint32_t biImageSize{ 0 }; // image size after compression - no compression so 0
	int32_t biXPixelsPerM{ 0 };
	int32_t biYPixelsPerM{ 0 };
	uint32_t biClrUsed{ 0 };
	uint32_t biClrImportant{ 0 };
};
#pragma pack(pop)

struct Pixel {
	uint8_t r, g, b;
};
