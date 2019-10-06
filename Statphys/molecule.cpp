#include <random>
#include <vector>
#include <iostream>
#include <memory>

constexpr double def_mean = 100.0;
constexpr double def_std = 200.0;
constexpr double def_left = -10.0;
constexpr double def_right = 10.0;
constexpr double def_down = -10.0;
constexpr double def_up = 10.0;

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

template<typename T, int p = 1>
struct SingletonWrapper : T
{
    SingletonWrapper(SingletonWrapper const&) = delete;
    void operator= (SingletonWrapper const&) = delete;
    static SingletonWrapper& getInstance(double value_1, double value_2)
    {
        static SingletonWrapper instance(value_1, value_2);
        return instance;
    }
private:
    SingletonWrapper(double value_1, double value_2) : T(value_1, value_2) {}
};

struct Molecule
{
private:
    double angle_by_coords(double x, double y)
    {
        return atan(y / x);
    }

    SingletonWrapper<NormalHandler>& normal;
    SingletonWrapper<UniformHandler>& uniform_x;
    SingletonWrapper<UniformHandler, 2>& uniform_y;

public:
    bool interacted = false;
    std::pair<double, double> position, velocity;

    Molecule(double left, double right, double down, double up)
            : normal{ SingletonWrapper<NormalHandler>::getInstance(def_mean, def_std) },
              uniform_x { SingletonWrapper<UniformHandler>::getInstance(left, right) },
              uniform_y { SingletonWrapper<UniformHandler, 2>::getInstance(down, up) },
              position { uniform_x, uniform_y },
              velocity { normal, normal }
    {
    }

    Molecule(const Molecule& mol)
            : normal{ mol.normal },
              uniform_x{ mol.uniform_x},
              uniform_y{ mol.uniform_y },
              position{ uniform_x, uniform_y },
              velocity{ normal, normal }
    {
    }

    double get_angle()
    {
        return angle_by_coords(velocity.first, velocity.second);
    }
};