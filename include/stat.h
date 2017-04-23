#ifndef STAT_H
#define STAT_H
#include <stdlib.h>
#include <math.h>

class Stat {
public:
	static float uniformRand() {
		return (float)rand()/(1.0 + RAND_MAX); 
	}


	/* mean m, standard deviation s */
	static float gaussianRand(float m, float s)	/* normal random variate generator */
	{				        
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
};

#endif