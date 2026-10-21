#include "advection/solver.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "advection/time_integrator.hpp"

namespace advection {

double solve(std::vector<double>& u, double dx, const SolverConfig& cfg) {
    if (cfg.a == 0.0) throw std::invalid_argument("solve: advection speed must be non-zero");

    const double dt_cfl = cfg.cfl * dx / std::abs(cfg.a);
    double t = 0.0;

    while (t < cfg.t_end) {
        const double dt = std::min(dt_cfl, cfg.t_end - t);  // ensuring overshoot does not occur
        u = forward_euler_step(u, cfg.a, dt, dx);
        t += dt;
    }
    return t;
}

}  // namespace advection
