#include <iostream>

int main() {
	const int imgWidth = 256;
	const int imgHeight = 256;

	std::cout << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";

	for (size_t i = 0; i < imgHeight; i++) {
		for (size_t j = 0; j < imgWidth; j++) {
			double r = (double)i / (imgWidth - 1);
			double g = (double)j / (imgHeight - 1);
			double b = 0.0;
			double rFormatted = 255.999 * r;
			double gFormatted = 255.999 * g;
			double bFormatted = 255.999 * b;

			std::cout << rFormatted << ' ' << gFormatted << ' ' << bFormatted << '\n';
		}
	}

	return 0;
}
