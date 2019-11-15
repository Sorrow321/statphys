#include <cmath>

class PoissonEstimator
{
    double sum;
    int n;
    double threshold;
public:
    void push(double value)
    {
        sum += value;
        n++;
    }

    bool test_value(double value)
    {
        return value <= sum / n * 2;
    }
};