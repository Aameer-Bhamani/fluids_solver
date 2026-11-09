#include "advection/solver.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "advection/time_integrator.hpp"
#include "advection/spatial_operator.hpp"
#include "advection/flux.hpp"

namespace advection {

double solve(std::vector<double>& u, const SolverConfig& cfg, const TimeIntegrator& method, const SpatialOperator& op) {
    if (cfg.a_x == 0.0) throw std::invalid_argument("solve: advection speed must be non-zero");

    const double dt_cfl = cfg.cfl * op.stable_dt();
    double t = 0.0;

    auto rhs = [&op](const std::vector<double>& state) {
        return op.apply(state);
    };

    while (t < cfg.t_end) {
        const double dt = std::min(dt_cfl, cfg.t_end - t);  // ensuring overshoot does not occur
        u = method.step(u, dt, rhs);
        t += dt;
    }
    return t;
}

}  // namespace advection
