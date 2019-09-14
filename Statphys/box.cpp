#include "molecule.cpp"
#include <chrono>
#include <future>
#include <thread>
#include <iomanip>

constexpr int ms_in_s = 1000;
constexpr int calc_ms = 20;
constexpr double def_radius = 0.1;

template<typename T>
class MutexWrapper
{
    std::mutex* sem;
    const T& resource;
public:
    MutexWrapper(std::mutex* sem, const T& resource)
        : sem(sem), resource(resource)
    {
        sem->lock();
    }
    ~MutexWrapper()
    {
        sem->unlock();
    }
    const T& get()
    {
        return resource;
    }
};

struct Box
{
private:

    double distance(const std::pair<double, double>& x, const std::pair<double, double>& y)
    {
        double u = x.first - y.first;
        double v = x.second - y.second;
        return u * u + v * v;
    }

    void calculate_positions()
    {
        auto [l, r, d, u] = bounds;
        for (size_t i = 0; i < molecules.size(); i++) {
            molecules[i].position.first = molecules[i].position.first 
                                            + (double(calculate_ms) / ms_in_s)
                                            * molecules[i].velocity.first;
            molecules[i].position.second = molecules[i].position.second
                                            + (double(calculate_ms) / ms_in_s)
                                            * molecules[i].velocity.second;
            
            // walls handler
            if(molecules[i].position.second < d || molecules[i].position.second > u) {
                molecules[i].velocity.second *= -1;
            }
            
            if(molecules[i].position.first < l || molecules[i].position.first > r) {
                molecules[i].velocity.first *= -1;
            }

            // interactions handler
            for (size_t j = i + 1; j < molecules.size(); j++) {
                if (distance(molecules[i].position, molecules[j].position) < 4 * radius * radius) {
                    std::swap(molecules[i].velocity, molecules[j].velocity);
                }
            }
        }
    }

    void box_think()
    {
        auto dt = std::chrono::milliseconds(calculate_ms);
        while (true)
        {
            std::this_thread::sleep_for(dt);
            sem.lock();
            calculate_positions();
            sem.unlock();
        }
    }

    std::mutex sem;
    double radius;
    std::tuple<double, double, double, double> bounds;
    std::vector<Molecule> molecules;
    std::future<void> calculate_thread;
    unsigned int calculate_ms;
public:
    Box(double radius = def_radius, std::tuple<double, double, double, double> bounds = { def_left, def_right, def_left, def_right },
        size_t molecules_num = 10,
        unsigned calc_ms = calc_ms) 
        : radius {radius},
          bounds(bounds),
          molecules (molecules_num, Molecule(std::get<0>(bounds), std::get<1>(bounds), std::get<2>(bounds), std::get<3>(bounds))),
          calculate_ms{calc_ms}
    {
        calculate_thread = std::async(std::launch::async, &Box::box_think, this);
    }

    const MutexWrapper<std::vector<Molecule>> get_molecules ()
    {
        return MutexWrapper<std::vector<Molecule>> (&sem, molecules);
    }
};

int main()
{
    Box b(0.1, { -100, 100, -1.0, 1.0 }, 20);
    std::cout << "hi" << std::endl;
    while (true) {
        {
            auto m = b.get_molecules();
            const std::vector<Molecule>& v = m.get();
            system("cls"); // system("clear"); in linux
            for (size_t i = 0; i < v.size(); i++) {
                std::cout << i << std::setprecision(4) << " x: " << v[i].position.first
                    << " \ty: " << v[i].position.second <<
                    " \tv_x: " << v[i].velocity.first << 
                    " \tv_y: " << v[i].velocity.second << std::endl;
            }
        }
        auto dt = std::chrono::milliseconds(20);
        std::this_thread::sleep_for(dt);
    }
}