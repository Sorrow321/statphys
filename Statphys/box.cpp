#include "molecule.cpp"
#include <chrono>
#include <future>
#include <thread>
#include <iomanip>

constexpr int ms_in_s = 1000;

struct Box
{
private:
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
            
            if(molecules[i].position.second < d || molecules[i].position.second > u) {
                molecules[i].velocity.second *= -1;
            }
            
            if(molecules[i].position.first < l || molecules[i].position.first > r) {
                molecules[i].velocity.first *= -1;
            }
        }
    }

    void box_think()
    {
        using namespace std::chrono_literals;
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
    unsigned int calculate_ms;
    unsigned int show_ms;
public:

    Box(std::tuple<double, double, double, double> bounds,
        size_t molecules_num,
        unsigned calc_ms = 10,
        unsigned show_ms = 30) 
        : bounds(bounds),
          molecules (molecules_num),
          calculate_ms{calc_ms},
          show_ms{show_ms}
    {
        using namespace std::chrono_literals;
        auto f = std::async(std::launch::async, &Box::box_think, this);
        auto dt = std::chrono::milliseconds(show_ms);
        while (f.wait_for(dt) == std::future_status::timeout) {
            system("cls");
            sem.lock();
            for (size_t i = 0; i < molecules.size(); i++) {
                std::cout << i << std::setprecision(4) << " x: " << molecules[i].position.first
                    << " \ty: " << molecules[i].position.second << std::endl;
            }
            sem.unlock();
            std::cout << std::endl;
        }
    }
};

int main()
{
    Box b({ -10.0, 10.0, -10.0, 10.0 }, 5);
}
