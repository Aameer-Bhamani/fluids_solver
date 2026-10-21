#pragma once

#include <vector>

namespace advection {

struct SolverConfig {
    double a      = 1.0;   ///< advection speed
    double cfl    = 0.5;   ///< CFL number
    double t_end  = 0.4;   ///< final time
};

/// Advance `u` in place to t_end. Returns the final time reached.
/// The last step is clipped so the solution lands exactly on t_end.
double solve(std::vector<double>& u, double dx, const SolverConfig& cfg);

}  // namespace advection
