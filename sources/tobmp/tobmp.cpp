#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "tobmp.h"
 

int writeBMP(const std::string& filename, std::stringstream& ppmData) {
    std::ofstream bmpFile(filename, std::ios::binary);
    if (!bmpFile) {
        std::cerr << "Failed to open bmp file: " << filename << std::endl;
        return -1;
    }

    std::string header;
    ppmData >> header;

    int width, height, maxColorValue;
    ppmData >> width >> height >> maxColorValue;

    BMPHeader bmpHeader;
    BMPInfoHeader bmpInfoHeader;

    bmpInfoHeader.biWidth = width;
    bmpInfoHeader.biHeight = -height;
    bmpInfoHeader.biImageSize = ((width * 3 + 3) & ~3) * height;
    bmpHeader.bfSize = bmpHeader.bfOffBits + bmpInfoHeader.biImageSize;

    bmpFile.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(bmpHeader));
    bmpFile.write(reinterpret_cast<const char*>(&bmpInfoHeader), sizeof(bmpInfoHeader));

    std::vector<uint8_t> row((width * 3 + 3) & ~3);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int r, g, b;
            ppmData >> r >> g >> b;
            row[x * 3] = static_cast<uint8_t>(b);  // BMP uses BGR format
            row[x * 3 + 1] = static_cast<uint8_t>(g);
            row[x * 3 + 2] = static_cast<uint8_t>(r);
        }
        bmpFile.write(reinterpret_cast<const char*>(row.data()), row.size());
    }

    return 0;
}