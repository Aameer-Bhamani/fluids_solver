#include "advection/time_integrator.hpp"

#include "advection/flux.hpp"

namespace advection {

std::vector<double> forward_euler_step(const std::vector<double>& u,
                                       double a, double dt, double dx) {
    const std::vector<double> div = upwind_divergence(u, a, dx);
    std::vector<double> result(u.size());
    for (std::size_t i = 0; i < u.size(); ++i) result[i] = u[i] - dt * div[i];
    return result;
}

}  // namespace advection
