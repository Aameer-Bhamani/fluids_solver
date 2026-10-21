#include "advection/flux.hpp"

#include <algorithm>

namespace advection {

std::vector<double> upwind_divergence(const std::vector<double>& u, double a, double dx) {
    const int n = static_cast<int>(u.size());
    std::vector<double> result(n);

    // Split velocity so the stencil follows the flow direction
    const double a_plus = std::max(a, 0.0);
    const double a_min  = std::min(a, 0.0);

    for (int i = 0; i < n; ++i) {
        const int im1 = (i - 1 + n) % n;  // periodic
        const int ip1 = (i + 1) % n;
        result[i] = (a_plus * (u[i] - u[im1]) + a_min * (u[ip1] - u[i])) / dx;
    }
    return result;
}

}  // namespace advection
