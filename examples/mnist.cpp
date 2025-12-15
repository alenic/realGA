#include "realga.h"

class FitnessMNIST : public FitnessFunction
{
public:
    FitnessMNIST() {

    }

    float eval(const RealChromosome &g) {
        return g.gene[0]*g.gene[0] + g.gene[1]*g.gene[1];
    }
private:
    vector<float> images;
};


int main() {


    

    return 0;
}