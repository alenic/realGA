#include "stat.h"
#include <random>
#include <thread>
#include <limits>
#include <stdexcept>

namespace
{
    // Thread-local random number generators
    thread_local std::mt19937 generator(std::random_device{}());
    thread_local std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);
    thread_local std::normal_distribution<float> normal_dist(0.0f, 1.0f);
    constexpr float MIN_SIGMA = 1e-6f;
}

float Stat::randUniform()
{
    return uniform_dist(generator);
}

float Stat::randUniform(float lb, float ub)
{
    if (lb >= ub)
    {
        throw std::invalid_argument("lb >= ub");
    }
    std::uniform_real_distribution<float> dist(lb, ub);
    return dist(generator);
}

int Stat::randIndex(int n)
{
    if (n <= 0)
    {
        throw std::invalid_argument("n <= 0");
    }
    std::uniform_int_distribution<int> dist(0, n - 1);
    return dist(generator);
}

float Stat::randGaussian(float mean, float sigma)
{
    // Ensure minimum sigma to avoid numerical issues
    if (sigma < MIN_SIGMA)
    {
        sigma = MIN_SIGMA;
    }

    // Use thread-local normal distribution
    normal_dist.param(std::normal_distribution<float>::param_type(mean, sigma));
    return normal_dist(generator);
}

void Stat::setSeed(unsigned int seed)
{
    generator.seed(seed);
    // Reset distributions to ensure consistent behavior
    uniform_dist.reset();
    normal_dist.reset();
}

void Stat::setSeedFromDevice()
{
    generator.seed(std::random_device{}());
    uniform_dist.reset();
    normal_dist.reset();
}

int Stat::randInteger(int min_val, int max_val)
{
    if (min_val >= max_val)
    {
        throw std::invalid_argument("min_val >= max_val");
    }
    std::uniform_int_distribution<int> dist(min_val, max_val);
    return dist(generator);
}
