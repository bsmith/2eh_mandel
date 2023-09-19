#include <cstdio>
#include <cmath>

static const int WIDTH = 512;
static const int HEIGHT = 512;
static const float X_MIN = -1.5 - 0.7;
static const float X_MAX = 1.5 - 0.7;
static const float Y_MIN = -1.5;
static const float Y_MAX = 1.5;
static const int ITERS = 1024;

void mandelbrot(float *output) {
	float x_step = (X_MAX - X_MIN) / (float)WIDTH;
	float y_step = (Y_MAX - Y_MIN) / (float)HEIGHT;

	for (int y_pixel = 0; y_pixel < HEIGHT; y_pixel++) {
		for (int x_pixel = 0; x_pixel < WIDTH; x_pixel++) {
			float c_x = X_MIN + ((float)x_pixel + 0.5) * x_step;
			float c_y = Y_MIN + ((float)y_pixel + 0.5) * y_step;

			float z_x = c_x;
			float z_y = c_y;
			for (int iter = 1; iter <= ITERS; iter++) {
				/* z <- z^2 + c */
				float z_x2 = z_x*z_x - z_y*z_y + c_x;
				float z_y2 = 2*z_x*z_y + c_y;
				z_x = z_x2;
				z_y = z_y2;
				if ((z_x > 2.) || (z_y > 2.))
					break;
			}

			if (sqrtf(z_x*z_x + z_y*z_y) < 2)
				output[y_pixel*WIDTH+x_pixel] = 0.;
			else
				output[y_pixel*WIDTH+x_pixel] = 1.;
		}
	}
}

int main(void) {
	float *data = new float[WIDTH * HEIGHT];

	mandelbrot(data);

	printf("P6\n");
	printf("# created by " __FILE__ "\n");
	printf("%d %d\n", WIDTH, HEIGHT);
	printf("255\n");

	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			unsigned char r = 180 * data[row*WIDTH+col];
			unsigned char g = 255 * (1 - data[row*WIDTH+col]);
			unsigned char b = 255 * (1 - data[row*WIDTH+col]);
			unsigned char buffer[3] = {r, g, b};
			fwrite(buffer, 1, 3, stdout);
		}
	}

	return 0;
}
