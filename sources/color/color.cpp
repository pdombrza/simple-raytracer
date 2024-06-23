#include "color.h"

void writeColorToStream(std::stringstream& out, const glm::vec3& pixelColor) {
	// writes color to stringstream

	int rTranslated = (int)(255.999 * pixelColor.x);
	int gTranslated = (int)(255.999 * pixelColor.y);
	int bTranslated = (int)(255.999 * pixelColor.z);

	out << rTranslated << ' ' << gTranslated << ' ' << bTranslated << '\n';
}