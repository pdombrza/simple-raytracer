#include "color.h"

void writeColorToStream(std::stringstream& out, const glm::vec3& pixelColor) {
	// writes color to stringstream

	int rTranslated = (int)(256 * std::clamp(pixelColor.x, 0.0f, 0.999f));
	int gTranslated = (int)(256 * std::clamp(pixelColor.y, 0.0f, 0.999f));
	int bTranslated = (int)(256 * std::clamp(pixelColor.z, 0.0f, 0.999f));

	out << rTranslated << ' ' << gTranslated << ' ' << bTranslated << '\n';
}