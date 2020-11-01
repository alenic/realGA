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
#ifdef _WIN32
  #include <Windows.h>
  #include <process.h>
#else
  #include <pthread.h>
#endif

#include "realgen.h"



class RealGenMultithread: public RealGen {
private:
  unsigned int nThread;
#ifdef _WIN32
  HANDLE *localThread;
#else
  pthread_t *localThread;
#endif
public:
  RealGenMultithread(RealGenOptions &opt, unsigned int nThread);
  ~RealGenMultithread();

#ifdef _WIN32
  static unsigned __stdcall evaluatePopulationThread(void *params);
#else
  static void *evaluatePopulationThread(void *params);
#endif
  
  void evolve();
};


struct thread_params {
  int startIndex;
  int endIndex;
  RealGenMultithread *ga;
};



#endif // REALGEN_H
