#include <utility>

struct Trace
{
private:
    std::pair<double, double> start;
    std::pair<double, double> direction;
    double radius;
public:
    Trace(std::pair<double, double> start, std::pair<double, double> direction, double radius)
        : start{ start }, direction{ direction }, radius{ radius }
    {
    }

    void intersect(const Trace& another)
    {

    }
};