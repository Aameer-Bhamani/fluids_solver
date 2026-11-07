#include "advection/solver.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "advection/time_integrator.hpp"
#include "advection/flux.hpp"

namespace advection {

double solve(std::vector<double>& u, double dx, const SolverConfig& cfg, const TimeIntegrator& method) {
    if (cfg.a == 0.0) throw std::invalid_argument("solve: advection speed must be non-zero");

    const double dt_cfl = cfg.cfl * dx / std::abs(cfg.a);
    double t = 0.0;

    auto rhs = [a = cfg.a, dx](const std::vector<double>& state) {
        std::vector<double> result = upwind_divergence(state, a, dx);
        for (size_t i = 0; i< state.size();++i){
            result[i] = -result[i];
        }
        return result;
    };

    while (t < cfg.t_end) {
        const double dt = std::min(dt_cfl, cfg.t_end - t);  // ensuring overshoot does not occur
        u = method.step(u, dt, rhs);
        t += dt;
    }
    return t;
}

}  // namespace advection
