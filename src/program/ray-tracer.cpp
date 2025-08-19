#include "ray-tracer.h"

const int WIDTH = 100;
const int HEIGHT = 50;

int main()
{
	Image img = Image(WIDTH, HEIGHT);

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			short red = static_cast<short>(std::round((double)j * 255. / (double)(WIDTH - 1)));
			short green = static_cast<short>(std::round((double)i * 255. / (double)(HEIGHT - 1)));
			img.draw(i, j, Color(red, green, 0));
		}
	}

	img.generateBmp("result.bmp");
	return 0;
}
