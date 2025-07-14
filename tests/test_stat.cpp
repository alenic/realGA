#include "testcommon.h"
#include <cmath>

StatTest::StatTest()
{
    Stat::setSeed(42);
    mVerbose = false;
}

StatTest::StatTest(bool verbose) : StatTest()
{
    mVerbose = verbose;
}

double StatTest::erfcinv(double x)
{
    if (x <= 0.0 || x >= 2.0)
    {
        throw std::domain_error("erfcinv(x) only valid for 0 < x < 2");
    }

    double pp = (x < 1.0) ? x : 2.0 - x;
    double t = std::sqrt(-2.0 * std::log(pp / 2.0));
    double result = -0.70711 * ((2.30753 + t * 0.27061) / (1.0 + t * (0.99229 + t * 0.04481)) - t);

    return (x < 1.0) ? result : -result;
}

double StatTest::chi_square_quantile_approx(int df, double alpha)
{
    // Approximation only valid for df >= 30
    double z = std::sqrt(2.0) * erfcinv(2 * alpha); // inverse CDF of normal
    double t = 2.0 / (9.0 * df);
    return df * std::pow(1.0 - t + z * std::sqrt(t), 3);
}

float StatTest::chiSquare(const int *observed, const float *expected, int bins)
{
    float chi2 = 0.0f;
    for (int i = 0; i < bins; ++i)
    {
        float diff = observed[i] - expected[i];
        if (expected[i] > 0)
            chi2 += (diff * diff) / expected[i];
    }
    return chi2;
}

void StatTest::test_randUniform(float lb, float ub)
{
    std::ostringstream oss;
    oss << "test_uniformRange" << "(lb=" << lb << ", ub=" << ub << ")";
    std::string test_name_str = oss.str();         // keep the string alive
    const char *test_name = test_name_str.c_str(); // now this is safe

    const int N = 100000;
    const int bins = 10;
    int observed[bins] = {0};
    float expected[bins];

    const float bin_width = (ub - lb) / bins;

    // Collect samples
    for (int i = 0; i < N; ++i)
    {
        float x = Stat::randUniform(lb, ub);
        int k = static_cast<int>((x - lb) / bin_width);
        // Handle possible edge case when x == ub
        if (k == bins)
            k = bins - 1;
        if (k >= 0 && k < bins)
            observed[k]++;
    }

    // Set expected frequency per bin
    for (int i = 0; i < bins; ++i)
        expected[i] = static_cast<float>(N) / bins;

    // Compute chi-squared statistic
    float chi = chiSquare(observed, expected, bins);

    // df = bins - 1 (no parameters estimated)
    int df = bins - 1;
    const float chi2_critical = chi_square_quantile_approx(df, 0.01f);

    if (chi <= chi2_critical)
    {
        coutColor(test_name, "PASSED", TEXT_GREEN);
    }
    else
    {
        coutColor(test_name, "FAILED", TEXT_RED);
        cout << test_name << ": Chi-squared = " << chi << " (threshold = " << chi2_critical << ") => ";
    }
}

void StatTest::test_randGaussian(float mean, float sigma)
{
    std::ostringstream oss;
    oss << "test_randGaussian" << "(mean=" << mean << ", std=" << sigma << ")";

    std::string test_name_str = oss.str();         // keep the string alive
    const char *test_name = test_name_str.c_str(); // now this is safe

    const int N = 100000;
    const int bins = 20;
    const float range_min = -3.0f;
    const float range_max = 3.0f;
    const float bin_width = (range_max - range_min) / bins;

    int observed[bins] = {0};
    float expected[bins] = {0};

    // Collect samples into bins
    for (int i = 0; i < N; ++i)
    {
        float x = Stat::randGaussian(mean, sigma);
        if (x >= range_min && x < range_max)
        {
            int bin = static_cast<int>((x - range_min) / bin_width);
            observed[bin]++;
        }
    }

    // Normal CDF using std::erf
    auto normal_cdf = [](double x, double mu, double sigma) -> double
    {
        return 0.5 * (1.0 + std::erf((x - mu) / (sigma * std::sqrt(2.0))));
    };

    // Calculate expected frequencies by integrating the PDF over each bin
    for (int i = 0; i < bins; ++i)
    {
        double a = range_min + i * bin_width;
        double b = a + bin_width;
        double p = normal_cdf(b, mean, sigma) - normal_cdf(a, mean, sigma);
        expected[i] = static_cast<float>(N * p);
    }

    // Compute chi-squared statistic
    float chi = chiSquare(observed, expected, bins);

    // df = bins - #of parameters(mean and sigma) - 1
    int df = bins - 2 - 1;
    const float chi2_critical = chi_square_quantile_approx(df, 0.01);

    if (chi <= chi2_critical)
    {
        coutColor(test_name, "PASSED", TEXT_GREEN);
    }
    else
    {
        coutColor(test_name, "FAILED", TEXT_RED);
        for (int i = 0; i < bins; ++i)
        {
            cout << i << ": " << observed[i] << "   " << expected[i] << endl;
        }
        cout << test_name << ": Chi-squared = " << chi
             << " (threshold = " << chi2_critical << ") => ";
    }
}

/*
void StatTest::test_randIndex()
{
    const int N = 100000; // Total trials
    const int K = 10;     // Number of bins (indices 0 to K-1)
    int counts[K] = {0};

    // Generate samples
    for (int i = 0; i < N; ++i)
    {
        int idx = Stat::randIndex(K);
        if (idx < 0 || idx >= K)
        {
            coutColor("test_randIndex", "FAILED: randIndex out of bounds\n", TEXT_YELLOW);
            return;
        }
        counts[idx]++;
    }

    // Optionally: print bin counts for inspection
    if (mVerbose)
    {
        cout << "Counts per bin:\n";
        for (int i = 0; i < K; ++i)
            cout << "  Bin " << i << ": " << counts[i] << endl;
    }

    // Compute chi-squared statistic
    float chi = uniformChiSquared(counts, K, N);

    // Critical value for chi-squared test at 95% confidence, df=K-1
    const float critical_value = 16.92f;

    if (mVerbose)
        cout << "Chi-squared statistic for randIndex(" << K << "): " << chi
             << " (threshold = " << critical_value << ") => ";

    if (chi <= critical_value)
    {
        coutColor("test_randIndex", "PASSED", TEXT_GREEN);
    }
    else
    {
        coutColor("test_randIndex", "FAILED", TEXT_RED);
    }
    cout << endl;
}
    */
/*
void StatTest::test_randInteger()
{
    int N = 100000;
    int min_val = 1, max_val = 6;
    int counts[6] = {0};

    for (int i = 0; i < N; ++i)
    {
        int r = Stat::randInteger(min_val, max_val);
        if (r < min_val || r > max_val)
        {
            coutColor("test_randInteger", "FAILED: randInteger out of bounds\n", TEXT_RED);
            return;
        }
        counts[r - min_val]++;
    }

    float chi = uniformChiSquared(counts, 6, N);
    float upper = 2 * sqrt(6);
    cout << "Chi squared for randInteger: " << chi << " <= " << upper << " ? ";
    if (chi <= upper)
    {
        coutColor("test_randInteger", "PASSED", TEXT_GREEN);
    }
    else
    {
        coutColor("test_randInteger", "FAILED", TEXT_RED);
    }
    cout << endl;
}
*/

void StatTest::test_smallSigmaGaussian()
{
    int N = 10000;
    float mean = 5.0f, small_sigma = 1e-10f;
    float sum = 0.0f;

    for (int i = 0; i < N; ++i)
    {
        float val = Stat::randGaussian(mean, small_sigma);
        sum += val;
    }

    float avg = sum / N;
    if (mVerbose)
        cout << "Mean of randGaussian with small sigma: " << avg << " (Expected ~" << mean << "): ";

    if (abs(avg - mean) < 0.1f)
    {
        coutColor("test_smallSigmaGaussian", "PASSED", TEXT_GREEN);
    }
    else
    {
        coutColor("test_smallSigmaGaussian", "FAILED", TEXT_RED);
    }
}
