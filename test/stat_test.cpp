#include "testcommon.h"

void test_uniformDist() {
	int N=100000;
	int a[] = {0,0,0,0,0,0,0,0,0,0};
	for(int i=0; i<N; i++) {
		int k = Stat::uniformRand()*9;
		a[k]++;
	}
	for(int i=0;i<10; i++) {
		cout << a[i] << endl;
	}
}
