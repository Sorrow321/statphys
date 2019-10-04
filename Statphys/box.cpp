#include "molecule.cpp"
#include <chrono>
#include <future>
#include <thread>
#include <iomanip>
#include <cmath>
#include <unordered_set>

constexpr int ms_in_s = 1000;
constexpr int calc_ms = 5;
constexpr size_t def_molnum = 50;
constexpr size_t def_obs = 2;
constexpr double def_radius = 1.0;
constexpr int def_interactions_num = 10;

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

    void walls_handler(size_t id)
    {
        auto[l, r, u, d] = bounds;
        if (molecules[id].position.second > d) {
            if (molecules[id].velocity.second > 0) {
                molecules[id].velocity.second *= -1;
            }
        }
        if (molecules[id].position.second < u) {
            if (molecules[id].velocity.second < 0) {
                molecules[id].velocity.second *= -1;
            }
        }

        if (molecules[id].position.first < l) {
            if (molecules[id].velocity.first < 0) {
                molecules[id].velocity.first *= -1;
            }
        }
        if (molecules[id].position.first > r) {
            if (molecules[id].velocity.first > 0) {
                molecules[id].velocity.first *= -1;
            }
        }
    }

    bool interact_cell(size_t id, int x, int y)
    {
        auto& lhs = molecules[id];
        for (auto it : grid[x][y]) {
            auto& rhs = *it;
            if (&rhs == &lhs) {
                continue;
            }
            if (distance(lhs.position, rhs.position) < 4 * radius * radius) {
                std::swap(lhs.velocity, rhs.velocity);
                if (id < observing_num) {
                    interactions[id]++;
                }
                return true;
            }
        }
        return false;
    }
    
    void calculate_positions()
    {
        for (size_t i = 0; i < molecules.size(); i++) {

            // moving the molecule
            molecules[i].position.first = molecules[i].position.first
                + (double(calculate_ms) / ms_in_s)
                * molecules[i].velocity.first;
            molecules[i].position.second = molecules[i].position.second
                + (double(calculate_ms) / ms_in_s)
                * molecules[i].velocity.second;

            // grid indexes
            int grid_x = int(molecules[i].position.first / (2 * radius));
            int grid_y = int(molecules[i].position.second / (2 * radius));
            
            if (grid_pos[i].first != grid_x || grid_pos[i].second != grid_y) {
                // if fast molecule flew out of bounds
                if (grid_x < 0 || grid_y < 0 || grid_x >= grid.size() || grid_y >= grid[0].size()) {
                    continue;
                }

                auto& prev = grid[grid_pos[i].first][grid_pos[i].second];
                prev.erase(prev.find(&molecules[i]));

                auto& cur = grid[grid_x][grid_y];
                cur.insert(&molecules[i]);

                grid_pos[i].first = grid_x;
                grid_pos[i].second = grid_y;
            }

            // walls handler
            walls_handler(i);

            // interactions handler
            for (int k = -1; k <= 1; k++) {
                bool _b = false;
                for (int p = -1; p <= 1; p++) {
                    int _i = grid_x + k;
                    int _j = grid_y + p;

                    if (_i >= 0 && _i < grid.size() && _j >= 0 && _j < grid[0].size()) {
                        if (interact_cell(i, _i, _j)) {
                            _b = true;
                            break;
                        }
                    }
                }
                if (_b) {
                    break;
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

    size_t observing_num;
    int interactions_num;
    std::mutex sem;
    double radius;
    std::tuple<double, double, double, double> bounds;
    std::vector<Molecule> molecules;
    std::future<void> calculate_thread;
    unsigned int calculate_ms;
    std::vector<std::pair<int, int>> grid_pos;
    std::vector<int> interactions;
    std::vector<std::vector<std::unordered_set<Molecule*>>> grid;
public:
    Box(double radius = def_radius,
        std::tuple<double, double, double, double> bounds = { def_left, def_right, def_left, def_right },
        size_t molecules_num = def_molnum,
        unsigned calc_ms = calc_ms,
        size_t observing_num = def_obs,
        int interactions_num = def_interactions_num)
        : observing_num{ observing_num },
          interactions_num{ interactions_num },
          radius{ radius },
          bounds(bounds),
          molecules(molecules_num, Molecule(std::get<0>(bounds), std::get<1>(bounds), std::get<2>(bounds), std::get<3>(bounds))),
          calculate_ms{ calc_ms },
          grid_pos(molecules_num),
          interactions(observing_num),
          grid(ceil(std::get<1>(bounds)), std::vector<std::unordered_set<Molecule*>>(ceil(std::get<3>(bounds))))
    {
        for (size_t i = 0; i < molecules_num; i++) {
            grid_pos[i].first = int(molecules[i].position.first / (2 * radius));
            grid_pos[i].second = int(molecules[i].position.second / (2 * radius));
            
            grid[grid_pos[i].first][grid_pos[i].second].insert(&molecules[i]);
        }
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

    void set_radius(double radius)
    {

    }

    void set_molecules_num(size_t molecules_num)
    {

    }
};
