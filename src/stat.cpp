/*
realGen: Genetic Algorithm with Real values

author: Alessandro Nicolosi
website: https://github.com/alenic
*/
#include "stat.h"


float Stat::randUniform() {
    float r = (float)rand() / (RAND_MAX + 1.0);
    return r;
}

float Stat::randUniform(float lb, float ub) {
    return (ub-lb)*randUniform() + lb;
}

int Stat::randIndex(int N) {
    return (int)(randUniform()*N);
}

/* mean m, standard deviation s */
float Stat::randGaussian(float m, float s) {

    if (s < MIN_SIGMA) {
        s = MIN_SIGMA;
    }

    float x1, x2, w, y1;
    static float y2;
    static int use_last = 0;

    if (use_last)
    {
        y1 = y2;
        use_last = 0;
    }
    else
    {
        do {
            x1 = 2.0 * (float)rand() / (RAND_MAX + 1.0) - 1.0;
            x2 = 2.0 * (float)rand() / (RAND_MAX + 1.0) - 1.0;
            w = x1 * x1 + x2 * x2;
        } while ( w >= 1.0 );

        w = sqrt( (-2.0 * log( w ) ) / w );
        y1 = x1 * w;
        y2 = x2 * w;
        use_last = 1;
    }

    return( m + y1 * s );
}


void Stat::setSeed(unsigned int seed) {
    srand(seed);
}


