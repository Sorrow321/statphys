#include "molecule.cpp"
#include <chrono>
#include <future>
#include <thread>
#include <iomanip>
#include <cmath>
#include <unordered_set>
#include <queue>

constexpr int ms_in_s = 1000;
constexpr int calc_ms = 5;
constexpr size_t def_molnum = 250;
constexpr size_t def_trajnum = 1;
constexpr double def_radius = 5.0;
constexpr int def_interactions_num = 5;
constexpr int def_mode = 1;
constexpr double def_trajlength = 500.0;
constexpr double def_obs = 1;
constexpr int queue_limit = 50000;

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

class PoissonEstimator
{
    double sum = 0;
    int n = 0;
public:
    void push(double value)
    {
        sum += value;
        n++;
    }

    bool test_value(double value)
    {
        return value <= sum / n * 2;
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

    void handle_interaction(size_t id)
    {
        sem_interacted.lock();
        interacted[id] = true;
        sem_interacted.unlock();

        interactions[id]++;


        sem_trajectory.lock();
        double dx = trajectory[id].first - molecules[id].position.first;
        double dy = trajectory[id].second - molecules[id].position.second;
        trajectory[id] = molecules[id].position;
        sem_trajectory.unlock();

        if (mode == 1) {
            lengths[id] += sqrt(dx * dx + dy * dy);

            if (lengths[id] >= trajectory_length) {
                sem_int_stats.lock();
                if (interaction_stats.size() <= queue_limit) {
                    interaction_stats.push(interactions[id] - 1);
                }
                sem_int_stats.unlock();

                lengths[id] = 0.0;
                interactions[id] = 0;

                sem_finished_lengths.lock();
                finished_lengths[id] = true;
                sem_finished_lengths.unlock();
            }
        }else if (mode == 2) {
            lengths[id] += sqrt(dx * dx + dy * dy);

            if (interactions[id] == interactions_num) {
                sem_len_stats.lock();
                if (len_stats.size() <= queue_limit) {
                    len_stats.push(lengths[id]);
                }
                sem_len_stats.unlock();

                lengths[id] = 0.0;
                interactions[id] = 0;

                sem_finished_interactions.lock();
                finished_interactions[id] = true;
                sem_finished_interactions.unlock();
            }
        }
    }

    void walls_handler(size_t id)
    {
        auto[l, r, u, d] = bounds;
        if (molecules[id].position.second > d) {
            if (molecules[id].velocity.second > 0) {
                molecules[id].velocity.second *= -1;
                handle_interaction(id);
                return;
            }
        }
        if (molecules[id].position.second < u) {
            if (molecules[id].velocity.second < 0) {
                molecules[id].velocity.second *= -1;
                handle_interaction(id);
                return;
            }
        }

        if (molecules[id].position.first < l) {
            if (molecules[id].velocity.first < 0) {
                molecules[id].velocity.first *= -1;
                handle_interaction(id);
                return;
            }
        }
        if (molecules[id].position.first > r) {
            if (molecules[id].velocity.first > 0) {
                molecules[id].velocity.first *= -1;
                handle_interaction(id);
                return;
            }
        }
    }

    bool interact_cell(size_t _i, int x, int y)
    {
        auto& lhs = molecules[_i];
        for (auto _j : grid[x][y]) {
            auto& rhs = molecules[_j];
            if (_i == _j) {
                continue;
            }
            if (distance(lhs.position, rhs.position) < 4 * radius * radius) {
                // avoid duplications
                if (_i > _j) {
                    continue;
                }
                // stucked molecules fix
                if (prev_interactions[_i] == _j) {
                    current_interactions[_i] = _j;
                    continue;
                }

                current_interactions[_i] = _j;
                std::swap(lhs.velocity, rhs.velocity);

                handle_interaction(_i);
                handle_interaction(_j);

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
                auto& cur = grid[grid_x][grid_y];
                prev.erase(prev.find(i));
                cur.insert(i);

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

        std::swap(current_interactions, prev_interactions);
        current_interactions = std::vector<size_t>(molecules.size(), -1);
    }

    void box_think()
    {
        auto dt = std::chrono::milliseconds(calculate_ms);
        while (true)
        {
            std::this_thread::sleep_for(dt);
            sem_molecules.lock();
            if (stop_thread) {
                return;
            }
            calculate_positions();
            sem_molecules.unlock();
        }
    }

    std::atomic<bool> stop_thread;
    bool started;
    int mode;
    int interactions_num;
    double trajectory_length;
    std::mutex sem_trajectory;
    std::mutex sem_len_stats;
    std::mutex sem_int_stats;
    std::mutex sem_molecules;
    std::mutex sem_interacted;
    std::mutex sem_finished_interactions;
    std::mutex sem_finished_lengths;
    double radius;
    std::tuple<double, double, double, double> bounds;
    std::vector<Molecule> molecules;
    unsigned int calculate_ms;
    std::vector<std::pair<int, int>> grid_pos;
    std::vector<int> interactions;
    std::vector<std::vector<std::unordered_set<size_t>>> grid;
    std::vector<size_t> prev_interactions;
    std::vector<size_t> current_interactions;
    std::vector<std::pair<double, double>> trajectory;
    std::vector<double> lengths;
    std::queue<double> len_stats;
    std::queue<double> interaction_stats;
    std::vector<bool> finished_interactions;
    std::vector<bool> finished_lengths;
    std::vector<bool> interacted;
    PoissonEstimator p_est;
    std::thread* calculate_thread;
public:
    /*
    mode (1 or 2): if 1, then the number of collisions per length is examined
                   if 2, then the trajectory length for the desired number of collisions is examined

    trajectory_length - trajectory length in 1st mode
    interactions_num - number of interactions in 2nd mode
    */
    Box(double radius = def_radius,
        std::tuple<double, double, double, double> bounds = { def_left, def_right, def_left, def_right },
        size_t molecules_num = def_molnum,
        unsigned calc_ms = calc_ms,
        int mode = def_mode,
        int interactions_num = def_interactions_num,
        double trajectory_length = def_trajlength)
            : stop_thread{false},
              started{false},
              mode {mode},
              interactions_num{ interactions_num },
              trajectory_length { trajectory_length },
              radius{ radius },
              bounds(bounds),
              molecules(molecules_num, Molecule(std::get<0>(bounds), std::get<1>(bounds), std::get<2>(bounds), std::get<3>(bounds))),
              calculate_ms{ calc_ms },
              grid_pos(molecules_num),
              interactions(molecules_num),
              grid(ceil(std::get<1>(bounds)), std::vector<std::unordered_set<size_t>>(ceil(std::get<3>(bounds)))),
              prev_interactions(molecules_num, -1),
              current_interactions(molecules_num, -1),
              trajectory(molecules_num),
              lengths(molecules_num),
              finished_interactions(molecules_num),
              finished_lengths(molecules_num),
              interacted(molecules_num),
              calculate_thread{nullptr}
    {
        double mid_x = (std::get<1>(bounds) - std::get<0>(bounds)) / 2;
        double mid_y = (std::get<3>(bounds) - std::get<2>(bounds)) / 2;
        // collisions in initial state isn't allowed
        while (true) {
            bool good = true;
            molecules[0].position = { mid_x, mid_y };
            for (size_t i = 0; i < molecules_num; i++) {
                for (size_t j = i + 1; j < molecules_num; j++) {
                    if (distance(molecules[i].position, molecules[j].position) < 4 * radius * radius) {
                        molecules[j].position = { molecules[j].uniform_x, molecules[j].uniform_y };
                        good = false;
                        break;
                    }
                }
                if (!good) {
                    break;
                }
            }
            if (good) {
                break;
            }
        }


        for (size_t i = 0; i < molecules_num; i++) {
            trajectory[i] = molecules[i].position;
        }

        for (size_t i = 0; i < molecules_num; i++) {
            grid_pos[i].first = floor(molecules[i].position.first / (2 * radius));
            grid_pos[i].second = floor(molecules[i].position.second / (2 * radius));

            grid[grid_pos[i].first][grid_pos[i].second].insert(i);
        }
    }

    ~Box()
    {
        stop_thread = true;
        sem_molecules.unlock();
        if (calculate_thread) {
            calculate_thread->join();
        }
    }

    const MutexWrapper<std::vector<Molecule>> get_molecules()
    {
        return MutexWrapper<std::vector<Molecule>>(&sem_molecules, molecules);
    }

    const MutexWrapper<std::vector<std::pair<double, double>>> get_trajectory()
    {
        return MutexWrapper<std::vector<std::pair<double, double>>>(&sem_trajectory, trajectory);
    }

    double get_last_len()
    {
        double value;
        sem_len_stats.lock();
        if (len_stats.empty()) {
            value = -1;
        } else {
            value = len_stats.front();
            len_stats.pop();
        }
        sem_len_stats.unlock();
        return value;
    }

    double get_last_interactions_num()
    {
        double value;
        sem_int_stats.lock();
        if (interaction_stats.empty()) {
            value = -1;
        } else {
            value = interaction_stats.front();
            p_est.push(value);
            if (!p_est.test_value(value)) {
                value = -1;
            }
            interaction_stats.pop();
        }
        sem_int_stats.unlock();
        return value;
    }

    void pause()
    {
        sem_molecules.lock();
    }

    void unpause()
    {
        sem_molecules.unlock();
        if (!started) {
            started = true;
            calculate_thread = new std::thread(&Box::box_think, this);
        }
    }

    bool get_interacted(size_t id)
    {
        sem_interacted.lock();
        bool value = interacted[id];
        interacted[id] = false;
        sem_interacted.unlock();
        return value;
    }

    bool get_finished_interactions(size_t id)
    {
        sem_finished_interactions.lock();
        bool value = finished_interactions[id];
        finished_interactions[id] = false;
        sem_finished_interactions.unlock();
        return value;
    }

    bool get_finished_length(size_t id)
    {
        sem_finished_lengths.lock();
        bool value = finished_lengths[id];
        finished_lengths[id] = false;
        sem_finished_lengths.unlock();
        return value;
    }
};