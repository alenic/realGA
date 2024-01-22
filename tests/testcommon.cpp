#include "testcommon.h"

void coutColor(const char * s, TextColor color) {
    switch(color) {
    case TEXT_RED:
        cout << "\033[31m" << s << "\033[0m";
        break;
    case TEXT_GREEN:
        cout << "\033[32m" << s << "\033[0m";
        break;
    case TEXT_YELLOW:
        cout << "\033[33m" << s << "\033[0m";
        break;
    }
    cout << endl;
}


void printDistribution(vector<float> &x, size_t bins, int nstars) {
    int *freq = new int(bins);
    for (int i=0; i<bins; ++i) freq[i] = 0;

    if(freq == NULL) {
        cerr << "Error! memory not allocated." << endl;
        exit(-1);
    }
    float minV = *std::min_element(x.begin(), x.end());
    float maxV = *std::max_element(x.begin(), x.end());
    float delta = maxV - minV;

    for (int i=0; i<x.size(); ++i) {
        float value = bins*(x[i] - minV) / delta;
        ++freq[int(value)];
    }

    for (int i=0; i<bins; ++i) {
        cout << ": ";
        cout << string(freq[i] * nstars/x.size(), '*') << endl;
    }
    cout << endl;
    delete[] freq;
}
