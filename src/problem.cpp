#include "advection/problem.hpp"

#include <cmath>

namespace advection {

namespace {
double pulse(double x) {
    const double d = x - 0.3;
    return 1.0 + 0.5 * std::exp(-100.0 * d * d);
}
}  // namespace

std::vector<double> initial_condition(const Grid1D& grid) {
    std::vector<double> u(grid.n_cells);
    for (int i = 0; i < grid.n_cells; ++i) u[i] = pulse(grid.x[i]);
    return u;
}

std::vector<double> analytic_solution(const Grid1D& grid, double a, double t) {
    std::vector<double> u(grid.n_cells);
    for (int i = 0; i < grid.n_cells; ++i) u[i] = pulse(grid.x[i] - a * t);
    return u;
}

}  // namespace advection
