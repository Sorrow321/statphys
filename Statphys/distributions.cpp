#include <cmath>
#include <vector>

unsigned factorial(unsigned n)
{
    if (n == 1)
        return 1;
    else
        return n * factorial(n - 1);
}


std::vector<std::vector<double>> distribution_1(int x, float radius, float volume, int count){
    std::vector<std::vector<double>> mas;
    for (int n = 1; n < 500; n++){
         double l_mean = 1 / (sqrt(2.0) * 3.1415 * (2*radius) * (2*radius) * (count / (volume * 2 * radius)));
         double result = 1;
         for (int i = 1; i <= n; i++){
             result = result * (x / l_mean) / i;
         }
         result = result * exp(-x / l_mean);
         if (result > 0.005){
            std::vector<double> mass;
            mass.push_back(n);
            mass.push_back(result);
            mas.push_back(mass);
         }
    }
    return mas;
}


std::vector<std::vector<double>> distribution_2(int n, float radius, float volume, float count){
    std::vector<std::vector<double>> mas;
    for (int x = 0; x < 10000; x++){
        double l_mean = 1 / (sqrt(2.0) * M_PI * (2*radius) * (2*radius) * (count / (volume * 2 * radius)));
        double result = (pow(x, n) * exp( (-1) * x / l_mean)) / (factorial(n-1) * pow(l_mean, n));
        if (result > 0.00001){
            std::vector<double> mass;
            mass.push_back(x);
            mass.push_back(result);
            mas.push_back(mass);
        }
    }
    return mas;
}