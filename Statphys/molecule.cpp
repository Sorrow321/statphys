#include <random>
#include <vector>
#include <iostream>
#include <memory>

constexpr double def_mean = 0.0;
constexpr double def_std = 2.0;
constexpr double def_left = -10.0;
constexpr double def_right = 10.0;

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
    NormalHandler(double mean = def_mean, double std = def_std)
        : d{ mean, std }
    {
    }
    double get()
    {
        return d(gen);
    }
};

struct UniformHandler : RandomHandler
{
    std::uniform_real_distribution<double> d;
    UniformHandler(double left = def_left, double right = def_right)
        : d{ left, right }
    {
    }
    double get()
    {
        return d(gen);
    }
};

template<typename T>
struct SingletonWrapper : T
{
    SingletonWrapper(SingletonWrapper const&) = delete;
    void operator= (SingletonWrapper const&) = delete;
    static SingletonWrapper& getInstance()
    {
        static SingletonWrapper instance;
        return instance;
    }
private:
    SingletonWrapper() {}
};

struct Molecule
{
private:
    double radius;
    SingletonWrapper<NormalHandler>& normal = SingletonWrapper<NormalHandler>::getInstance();
    SingletonWrapper<UniformHandler>& uniform = SingletonWrapper<UniformHandler>::getInstance();

public:
    std::pair<double, double> position, velocity;
    Molecule(double radius = 0.1) 
        : radius { radius },
          position { uniform, uniform },
          velocity { normal, normal }
    {
    }
    Molecule(std::pair<double, double> position, std::pair<double, double> velocity)
        : position{ position }, velocity{ velocity } { }
};
