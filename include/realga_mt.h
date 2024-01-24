/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        This is the RealGA multithreading implementation.
        Here threads are used to compute the fitness function
        in parallel.

        TODO: parallelize the evolve algorithm
---------------------------------------------
*/
#ifndef REALGA_MULTITHREAD_H
#define REALGA_MULTITHREAD_H
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

#include "realga.h"

class RealGenMultithread: public RealGA {
private:
    unsigned int nThread;
#ifdef _WIN32
    HANDLE *localThread;
#else
    pthread_t *localThread;
#endif
public:
    RealGenMultithread(unsigned int nThread);
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



#endif // REALGA_MULTITHREAD_H
