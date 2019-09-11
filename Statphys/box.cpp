#include "molecule.cpp"
#include <chrono>
#include <future>
#include <thread>

// testing vs commit
struct Box
{
private:
    void calculate_positions()
    {
        for (size_t i = 0; i < molecules.size(); i++) {
            molecules[i].position.first = molecules[i].position.first + dt * molecules[i].velocity.first;
            molecules[i].position.second = molecules[i].position.second + dt * molecules[i].velocity.second;
        }
    }

    void box_think()
    {
        using namespace std::chrono_literals;
        while (true)
        {
            std::this_thread::sleep_for(200ms);
            calculate_positions();
        }
    }

    std::mutex sem;
    std::tuple<double, double, double, double> bounds;
    std::vector<Molecule> molecules;
    double dt = 1;
public:

    Box(std::tuple<double, double, double, double> bounds, size_t molecules_num) 
        : bounds(bounds), molecules (molecules_num)
    {
        using namespace std::chrono_literals;
        auto f = std::async(std::launch::async, &Box::box_think, this);
        while (f.wait_for(500ms) == std::future_status::timeout) {
            sem.lock();
            system("cls");
            for (size_t i = 0; i < molecules.size(); i++) {
                std::cout << i << " " << "x: " << molecules[i].position.first << " y: " << molecules[i].position.second << std::endl;
            }
            std::cout << std::endl;
            sem.unlock();
        }
    }
};

int main()
{
    Box b({ -100.0, 100.0, -100.0, 100.0 }, 5);
}