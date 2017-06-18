#include "stat.h"

Stat::Stat() {
	//gen.seed(rd());
}

float Stat::uniformRand() {
	//std::uniform_real_distribution<> dist(0.0, 1.0);
	//return dist(gen);
	return (float)rand()/(RAND_MAX + 1.0); 
}

int Stat::uniformIndex(int N) {
	return rand() % N;
}


float Stat::uniformRand(float lb, float ub) {
	return (ub-lb)*uniformRand() + lb;
}

/* mean m, standard deviation s */
float Stat::gaussianRand(float m, float s) {
	float x1, x2, w, y1;
	static float y2;
	static int use_last = 0;

	if (use_last)		        /* use value from previous call */
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
			x1 = 2.0 * uniformRand() - 1.0;
			x2 = 2.0 * uniformRand() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}