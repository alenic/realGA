#include "testcommon.h"

void coutColor(const char *name, const char *s, TextColor color)
{
    switch (color)
    {
    case TEXT_RED:
        cout << name << " " << "\033[31m" << s << "\033[0m";
        break;
    case TEXT_GREEN:
        cout << name << " " << "\033[32m" << s << "\033[0m";
        break;
    case TEXT_YELLOW:
        cout << name << " " << "\033[33m" << s << "\033[0m";
        break;
    }
    cout << endl;
}

void printDistribution(vector<float> &x, int bins, int nstars)
{
    int *freq = new int[bins];
    for (int i = 0; i < bins; ++i)
        freq[i] = 0;

    if (freq == NULL)
    {
        cerr << "Error! memory not allocated." << endl;
        exit(-1);
    }

    float minV = *std::min_element(x.begin(), x.end());
    float maxV = *std::max_element(x.begin(), x.end());
    float delta = maxV - minV;

    for (size_t i = 0; i < x.size(); ++i)
    {
        int value = (int)(bins * (x[i] - minV) / delta);
        if (value == bins)
            value--;
        ++freq[value];
    }

    for (int i = 0; i < bins; ++i)
    {
        cout << ": ";
        cout << string(freq[i] * nstars / x.size(), '*') << endl;
    }
    cout << endl;

    delete[] freq;
}
