#ifndef REALGENMULTITHREAD_H
#define REALGENMULTITHREAD_H
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <pthread.h>

#include "realgenotype.h"
#include "stat.h"
#include "realgen.h"



class RealGenMultithread: public RealGen {
private:
	unsigned int nThread;
	pthread_t *localThread;
public:
	RealGenMultithread(int np, int nx, float *lb, float *ub, unsigned int nThread);
	RealGenMultithread(int np, int nx, float *lb, float *ub, GAOptions opt,  unsigned int nThread);
	~RealGenMultithread();
	static void *evaluatePopulationThread(void *params);
	void evolve();
};


struct thread_params {
	int startIndex;
	unsigned int neval;
	RealGenMultithread *ga;
};



#endif // REALGEN_H