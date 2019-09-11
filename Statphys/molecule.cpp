#pragma once

#include <random>
#include <vector>
#include <iostream>
#include <memory>

struct RandomHandler
{
    virtual double get() = 0;
    virtual ~RandomHandler() {};
    operator double()
    {
        return get();
    }
protected:
    std::random_device rd{};
    std::mt19937 gen{ rd() };
};


struct NormalHandler : RandomHandler
{
    std::normal_distribution<double> d;
    NormalHandler(double mean, double std) : d{ mean, std } { }
    double get()
    {
        return d(gen);
    }
};

struct UniformHandler : RandomHandler
{
    std::uniform_real_distribution<double> d;
    UniformHandler(double left, double right) : d{ left, right } {}
    double get()
    {
        return d(gen);
    }
};

struct Molecule
{
private:
    NormalHandler normal{ 0.0, 1.0 };
    UniformHandler uniform{ 0.0, 10.0 };
public:
    std::pair<double, double> position, velocity;
    Molecule() : position{ uniform, uniform }, velocity{ normal, normal } { }
    Molecule(std::pair<double, double> position, std::pair<double, double> velocity)
        : position{ position }, velocity{ velocity } {  }
};