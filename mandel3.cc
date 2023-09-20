#include <cstdio>
#include <cmath>
#include <omp.h>

static const int WIDTH = 4096;
static const int HEIGHT = 4096;
static const float X_MIN = -1.5 - 0.7;
static const float X_MAX = 1.5 - 0.7;
static const float Y_MIN = -1.5;
static const float Y_MAX = 1.5;
static const int ITERS = 1024;
static const int PAD = 4;

void mandelbrot(float *output) {
	float x_step = (X_MAX - X_MIN) / (float)WIDTH;
	float y_step = (Y_MAX - Y_MIN) / (float)HEIGHT;

#pragma omp parallel
	{
		if (omp_get_thread_num() == 0)
			fprintf(stderr, "using %d threads\n", omp_get_num_threads());
		//int rows_per_thread = HEIGHT / omp_get_num_threads();
		//int start_row = omp_get_thread_num() * rows_per_thread;
		//int end_row = start_row + rows_per_thread; /* use < */
		//if (omp_get_thread_num() == omp_get_num_threads() - 1)
		//	end_row = HEIGHT;
		//fprintf(stderr, "%d: %d -> %d\n", omp_get_thread_num(), start_row, end_row);
		//for (int y_pixel = start_row; y_pixel < end_row; y_pixel++) {
		int nthreads = omp_get_num_threads();
		int id = omp_get_thread_num();
		for (int y_pixel = id; y_pixel < HEIGHT; y_pixel += nthreads) {
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
					//output[y_pixel*WIDTH+x_pixel] = 0.;
					output[PAD*(y_pixel*WIDTH+x_pixel)] = sqrtf(z_x*z_x+z_y*z_y) / 2;
				else
					output[PAD*(y_pixel*WIDTH+x_pixel)] = 1.;
			}
		}
	}
}

int main(void) {
	float *data = new float[WIDTH * HEIGHT * PAD];

/*#pragma omp parallel num_threads(16)
	{
		int ID = omp_get_thread_num();
		fprintf(stderr, "hello world (%d)\n", ID);
	} */

	mandelbrot(data);

	fprintf(stderr, "mandelbrot complete\n");

	printf("P6\n");
	printf("# created by " __FILE__ "\n");
	printf("%d %d\n", WIDTH, HEIGHT);
	printf("255\n");

	unsigned char row_buffer[WIDTH * 3];
	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			unsigned char r = 180 * data[PAD*(row*WIDTH+col)];
			unsigned char g = 255 * (1 - data[PAD*(row*WIDTH+col)]);
			unsigned char b = 255 * (1 - data[PAD*(row*WIDTH+col)]);
			/*unsigned char buffer[3] = {r, g, b};
			fwrite(buffer, 1, 3, stdout);*/
			row_buffer[col * 3 + 0] = r;
			row_buffer[col * 3 + 1] = g;
			row_buffer[col * 3 + 2] = b;
		}
		fwrite(row_buffer, WIDTH, 3, stdout);
	}

	return 0;
}
