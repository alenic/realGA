/*
---------------------------------------------
    author: Alessandro Nicolosi
    website: https://github.com/alenic
    license: MIT

    file description:
        This is the RealGA multithreading implementation.
        Threads are used to compute the fitness function
        in parallel.
---------------------------------------------
*/
#ifndef REALGA_MULTITHREAD_H
#define REALGA_MULTITHREAD_H

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "realga.h"

class RealGAMultithread : public RealGA
{
private:
    unsigned int mNthread;

public:
    RealGAMultithread(unsigned int nThread);
    ~RealGAMultithread();

    void evolve() override;
};

#endif // REALGA_MULTITHREAD_H
