#include "color.h"

float linearToGamma(float linearComponent) {
	return (linearComponent > 0) ? std::sqrt(linearComponent) : 0.0f;
}

void writeColorToStream(std::stringstream& out, const glm::vec3& pixelColor) {
	// writes color to stringstream
	float r = linearToGamma(pixelColor.x);
	float g = linearToGamma(pixelColor.y);
	float b = linearToGamma(pixelColor.z);

	int rTranslated = (int)(256 * std::clamp(r, 0.0f, 0.999f));
	int gTranslated = (int)(256 * std::clamp(g, 0.0f, 0.999f));
	int bTranslated = (int)(256 * std::clamp(b, 0.0f, 0.999f));

	out << rTranslated << ' ' << gTranslated << ' ' << bTranslated << '\n';
}