#include "molecule.cpp"
#include <chrono>
#include <future>
#include <thread>
#include <iomanip>

constexpr int ms_in_s = 1000;

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

    double distance(std::pair<double, double> x, std::pair<double, double> y)
    {
        double u = x.first - y.first;
        double v = x.second - y.second;
        return sqrt(u * u + v * v);
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
    std::tuple<double, double, double, double> bounds;
    std::vector<Molecule> molecules;
    std::future<void> calculate_thread;
    unsigned int calculate_ms;
    unsigned int show_ms;
public:
    Box(std::tuple<double, double, double, double> bounds = { def_left, def_right, def_left, def_right },
        size_t molecules_num = 30,
        unsigned calc_ms = 10,
        unsigned show_ms = 30) 
        : bounds(bounds),
          molecules (molecules_num),
          calculate_ms{calc_ms},
          show_ms{show_ms}
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
    Box b;
    std::cout << "hi" << std::endl;
    while (true) {
        {
            auto m = b.get_molecules();
            const std::vector<Molecule>& v = m.get();
            system("cls"); // system("clear"); in linux
            for (size_t i = 0; i < v.size(); i++) {
                std::cout << i << std::setprecision(4) << " x: " << v[i].position.first
                    << " \ty: " << v[i].position.second << std::endl;
            }
        }
        auto dt = std::chrono::milliseconds(20);
        std::this_thread::sleep_for(dt);
    }
}