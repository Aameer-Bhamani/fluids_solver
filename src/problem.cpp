#include "advection/problem.hpp"


#include <cmath>

namespace advection {

namespace {

// Pulse parameters (TODO: move to the config)
constexpr double kBase   = 1.0;
constexpr double kAmp    = 0.5;
constexpr double kX0     = 0.3;
constexpr double kY0     = 0.3;
constexpr double kWidth  = 100.0;

// Map x into the periodic interval [lo, hi).
double wrap_into(double x, double lo, double hi) {
    const double L = hi - lo;
    double s = std::fmod(x - lo, L);
    if (s < 0.0) s += L;
    return lo + s;
}

// Pulse as a function of position (the same shape as the 1D pulse).
double pulse(double x, double y) {
    const double d = x - kX0;
    const double e = y - kY0;
    return kBase + kAmp * std::exp(-kWidth * d * d - kWidth * e * e);
}

}  // namespace

std::vector<double> analytic_solution(const Grid2D& grid, double a_x, double a_y, double t) {
    std::vector<double> u(grid.size());
    // n_y == 1 means a 1D run: drop the y dependence so the 1D result is unchanged.
    const bool is_1d = (grid.n_y == 1);

    for (int j = 0; j < grid.n_y; ++j) {
        for (int i = 0; i < grid.n_x; ++i) {
            const double xs = wrap_into(grid.x[i] - a_x * t, grid.x_left, grid.x_right);
            const double ys = is_1d ? kY0
                                    : wrap_into(grid.y[j] - a_y * t, grid.y_bottom, grid.y_top);
            u[grid.idx(i, j)] = pulse(xs, ys);
        }
    }
    return u;
}

std::vector<double> initial_condition(const Grid2D& grid) {
    return analytic_solution(grid, 0.0, 0.0, 0.0);
}

}  // namespace advection