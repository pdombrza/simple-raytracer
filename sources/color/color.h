#pragma once

#include <sstream>
#include <algorithm>
#include <vector>
#include <glm/glm.hpp>


float linearToGamma(float linearComponent);
void writeColorToStream(std::stringstream& out, const glm::vec3& pixelColor);
std::stringstream writeBufferToStream(std::unique_ptr<glm::vec3[]> pxBuffer, int imgWidth, int imgHeight);