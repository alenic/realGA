#include "testcommon.h"

void test_uniformDist();
void test_real_sphere(GAOptions);
void test_rosenbrock(GAOptions);


int main() {
	GAOptions opt;
	test_uniformDist();
	opt.mutation.type = UNIFORM_MUTATION;
	test_real_sphere(opt);
	opt.mutation.type = GAUSSIAN_MUTATION;
	test_real_sphere(opt);
	opt.mutation.type = UNIFORM_MUTATION;
	test_rosenbrock(opt);
	opt.mutation.type = GAUSSIAN_MUTATION;
	test_rosenbrock(opt);
	return 0;
}