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
		cout << "False" << endl;
	}
}

void test_gaussianDist(int mean, int stDev) {
	Stat stat;
	const int nrolls=10000;  // number of experiments
	const int nstars=100;    // maximum number of stars to distribute

	int p[10]={};
	for (int i=0; i<nrolls; ++i) {
		double number = stat.gaussianRand(mean, stDev);
		if ((number>=0.0)&&(number<10.0)) ++p[int(number)];
	}
	cout << "normal_distribution ("<< mean << "," << stDev << "):" << endl;
	for (int i=0; i<10; ++i) {
		cout << i << "-" << (i+1) << ": ";
		cout << string(p[i]*nstars/nrolls,'*') << endl;
	}

}