#pragma once

#include <sstream>
#include <algorithm>
#include <glm/glm.hpp>


float linearToGamma(float linearComponent);
void writeColorToStream(std::stringstream& out, const glm::vec3& pixelColor);