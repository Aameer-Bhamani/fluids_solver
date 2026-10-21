#pragma once

#include <vector>

#include "advection/grid.hpp"

namespace advection {

/// Gaussian pulse on a unit background: u0(x) = 1 + 0.5 exp(-100 (x - 0.3)^2)
std::vector<double> initial_condition(const Grid1D& grid);

/// Exact solution for periodic advection at speed `a` and time `t`.
/// (Valid while the pulse has not wrapped around the periodic boundary.)
std::vector<double> analytic_solution(const Grid1D& grid, double a, double t);

}  // namespace advection
