#include "testcommon.h"

float uniformChiSquared(int *a, int n, int N) {
	// n: number of intervals 
	// N: number of experiment
	float chi = 0.0;
	float expected = (float)N / (float)n;

	for (int i=0; i<n; i++) {
		float d = (a[i] - expected);
		chi += d * (d / expected);
	}
	return chi;
}

void test_uniformDist() {
	Stat stat;
	int N=100000;
	int a[] = {0,0,0,0,0,0,0,0,0,0};
	for(int i=0; i<N; i++) {
		int k = (int)(stat.uniformRand()*10);
		a[k]++;
	}
	float chi = uniformChiSquared(a, 10, N);
	float upper = 2*sqrt(10);
	cout << "Chi squared value: " << chi << " <= 2*sqrt(10) = " << upper << " ? ";
	if(chi <= upper) {
		cout << "True" << endl;
	} else {
		cout << "Flase" << endl;
	}
}

void test_gaussianDist() {
	Stat stat;
	int N=1000;
	for(int i=0; i<N; i++) {
		//cout << stat.gaussianRand(0,1) << ",";
	}
}