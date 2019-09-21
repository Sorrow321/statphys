#include "molecule.cpp"
#include <chrono>
#include <future>
#include <thread>
#include <iomanip>

constexpr int ms_in_s = 1000;
constexpr int calc_ms = 1;
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

    /*
    inline double dot(const std::pair<double, double>& x, const std::pair<double, double>& y)
    {
        return x.first * y.first + x.second + y.second;
    }*/

    inline void interact(size_t i, size_t j)
    {
        std::swap(molecules[i].velocity, molecules[j].velocity);
    }

    void calculate_positions()
    {
        auto[l, r, u, d] = bounds;
        for (size_t i = 0; i < molecules.size(); i++) {
            
            // moving the molecule
            molecules[i].position.first = molecules[i].position.first
                + (double(calculate_ms) / ms_in_s)
                * molecules[i].velocity.first;
            molecules[i].position.second = molecules[i].position.second
                + (double(calculate_ms) / ms_in_s)
                * molecules[i].velocity.second;

                
            // walls handler
            if (molecules[i].position.second > d) {
                if (molecules[i].velocity.second > 0) {
                    molecules[i].velocity.second *= -1;
                }
            }
            if (molecules[i].position.second < u) {
                if (molecules[i].velocity.second < 0) {
                    molecules[i].velocity.second *= -1;
                }
            }

            if (molecules[i].position.first < l) {
                if (molecules[i].velocity.first < 0) {
                    molecules[i].velocity.first *= -1;
                }
            }
            if (molecules[i].position.first > r) {
                if (molecules[i].velocity.first > 0) {
                    molecules[i].velocity.first *= -1;
                }
            }
            
            // interactions handler
            for (size_t j = i + 1; j < molecules.size(); j++) {
                if (distance(molecules[i].position, molecules[j].position) < 4 * radius * radius) {
                    interact(i, j);
                }
            }
        }
    }

    void show_traces()
    {

    }

    void box_think()
    {
        auto dt = std::chrono::milliseconds(calculate_ms);
        while (true)
        {
            std::this_thread::sleep_for(dt);
            sem.lock();
            calculate_positions();
            show_traces();
            sem.unlock();
        }
    }

    std::mutex sem;
    double radius;
    std::tuple<double, double, double, double> bounds;
    std::vector<Molecule> molecules;
    std::future<void> calculate_thread;
    unsigned int calculate_ms;
    bool is_active;
    //int observing;
    //bool observed;
    //std::vector<std::pair<double, double>> observing_pos;
public:
    Box(double radius = def_radius, std::tuple<double, double, double, double> bounds = { def_left, def_right, def_left, def_right },
        size_t molecules_num = 10,
        unsigned calc_ms = calc_ms)
        : radius{ radius },
          bounds(bounds),
          molecules(molecules_num, Molecule(std::get<0>(bounds), std::get<1>(bounds), std::get<2>(bounds), std::get<3>(bounds))),
          calculate_ms{ calc_ms }
    {
        calculate_thread = std::async(std::launch::async, &Box::box_think, this);
    }

    const MutexWrapper<std::vector<Molecule>> get_molecules()
    {
        return MutexWrapper<std::vector<Molecule>>(&sem, molecules);
    }
    
    void pause()
    {
        sem.lock();
    }
    
    void unpause()
    {
        sem.unlock();
    }
};
