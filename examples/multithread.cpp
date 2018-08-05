#include <iostream>
#include <time.h>
#ifndef _WIN32
  #include <sys/time.h>
#endif
#include <stdio.h>
#include "realgenmultithread.h"

#define N_SAMPLE 5000

using namespace std;

typedef unsigned long long timestamp_t;

#ifdef _WIN32


#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone
{
	int  tz_minuteswest; /* minutes W of Greenwich */
	int  tz_dsttime;     /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
	FILETIME ft;
	unsigned __int64 tmpres = 0;
	static int tzflag = 0;

	if (NULL != tv)
	{
		GetSystemTimeAsFileTime(&ft);

		tmpres |= ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |= ft.dwLowDateTime;

		tmpres /= 10;  /*convert into microseconds*/
					   /*converting file time to unix epoch*/
		tmpres -= DELTA_EPOCH_IN_MICROSECS;
		tv->tv_sec = (long)(tmpres / 1000000UL);
		tv->tv_usec = (long)(tmpres % 1000000UL);
	}

	if (NULL != tz)
	{
		if (!tzflag)
		{
			_tzset();
			tzflag++;
		}
		tz->tz_minuteswest = _timezone / 60;
		tz->tz_dsttime = _daylight;
	}

	return 0;
}
#endif

static timestamp_t get_timestamp() {
	struct timeval now;
	gettimeofday(&now, NULL);
	return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

float LB[] = {-5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0, -5.0};  // Lower bound of genes
float UB[] = { 5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0};  // Upper bound of genes

float A[N_SAMPLE][10];
float b[N_SAMPLE];


double myFitnessFunction(RealGenotype &g, void *par) {
  double f = 0.0;
  float Ag[N_SAMPLE];
  
  // Compute A*g
  for(int i=0; i<N_SAMPLE; ++i) {
    Ag[i] = 0;
    for(int j=0; j<10; j++) {
      Ag[i] += A[i][j]*g.gene[j];
    }
  }

  // Compute A*g-b
  float M = -1;
  for(int i=0; i<N_SAMPLE; ++i) {
    Ag[i] = Ag[i] - b[i];
    if(fabs(Ag[i]) > M) {
      M = fabs(Ag[i]);
    }
  }

  // Compute norm2
  for(int i=0; i<N_SAMPLE; ++i) {
    float uAg = Ag[i]/M;
    f += uAg*uAg;
  }

  return fabs(M)*sqrt(f);
}

void generateData() {
  Stat stat;
  float par[] = {1.0, -1.0, 0.5, -0.5, -2.5, 1.0, 2.5, -1.5, -2.0, 4.0};

  for(int i=0; i<N_SAMPLE; ++i) {
    b[i] = 0;
    for(int j=0; j<10; ++j) {
      if (j % 2 == 0) {
        A[i][j] = cos(j*((float)i/N_SAMPLE));
      } else {
        A[i][j] = sin(j*((float)i/N_SAMPLE));
      }
    }
  }

  for(int i=0; i<N_SAMPLE; ++i) {
    b[i] = 0;
    for(int j=0; j<10; ++j) {
      b[i] += par[j]*A[i][j];
    }
  }
}

void optimize(unsigned int seed, unsigned int nThreads) {
  RealGenOptions options;
  options.setGenesNumber(10);
  options.setPopulationSize(200);
  options.setBounds(LB, UB);
  options.setFitnessFunction(myFitnessFunction, NULL);
  options.setMaxGenerations(1500);
  options.setMutationType("gaussian");
  options.setMutationRate(0.05);
  options.setMutationGaussianScaleShrink(1, 1);

  RealGen *ga;
  
  if (nThreads==1) {
    ga = new RealGen(options);
  } else {
    ga = new RealGenMultithread(options, nThreads);
  }
  
  ga->setSeed(seed);

  // Init population with uniform random genes between LB and UB
  ga->initRandom();
  // Evolve the population for 1500 times
  for (int i=0; i<1500; i++) {
    ga->evolve();
    RealGenotype best = ga->getBestChromosome();
  }
  // get the best score function (the minimum)
  RealGenotype best = ga->getBestChromosome();
  // Print results
  cout << "Best solution: "<< best.toString() << endl;
  cout << "Best Fitness value = " << best.fitness << endl;
  delete ga;
}


int main(int argc,  char** argv) {
  timestamp_t t0, t1;
  unsigned int seed = 45;

  // Used for generate A matrix and b vector
  generateData();
  
  cout << "Single optimization...."<< endl;
  t0 = get_timestamp();
  optimize(seed, 1);
  t1 = get_timestamp();
  cout << "Eapsed Time: " << (t1-t0)/1000000.0L << endl;
  
  cout << "Multithread optimization (2 Threads)...."<< endl;
  t0 = get_timestamp();
  optimize(seed, 2);
  t1 = get_timestamp();
  cout << "Eapsed Time: " << (t1-t0)/1000000.0L << endl;
  
  cout << "Multithread optimization (4 Threads)...."<< endl;
  t0 = get_timestamp();
  optimize(seed, 4);
  t1 = get_timestamp();
  cout << "Eapsed Time: " << (t1-t0)/1000000.0L << endl;

  cout << "Multithread optimization (8 Threads)...."<< endl;
  t0 = get_timestamp();
  optimize(seed, 8);
  t1 = get_timestamp();
  cout << "Eapsed Time: " << (t1-t0)/1000000.0L << endl;

  return 0;
}

