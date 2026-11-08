#pragma once

#include <vector>
#include <string>

#include "advection/time_integrator.hpp"
#include "advection/spatial_scheme.hpp"

namespace advection {

struct SolverConfig {
    double a      = 1.0;   ///< advection speed
    double cfl    = 0.5;   ///< CFL number
    double t_end  = 0.4;   ///< final time
    std::string integrator = "forward_euler"; ///default time integrator
    std::string scheme = "upwind"; ///default spatial scheme
    std::string limiter = "minmod"; ///default limiter for MUSCL scheme
};

/// Advance `u` in place to t_end. Returns the final time reached.
/// The last step is clipped so the solution lands exactly on t_end.
double solve(std::vector<double>& u, double dx, const SolverConfig& cfg, const TimeIntegrator& method, const SpatialScheme& scheme);

}  // namespace advection
