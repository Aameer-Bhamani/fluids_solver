#pragma once

#include <vector>

#include "advection/grid2d.hpp"

namespace advection {

/// Gaussian pulse on a unit background: u0(x) = 1 + 0.5 exp(-100 (x - 0.3)^2)
std::vector<double> initial_condition(const Grid2D& grid);

/// Exact solution for periodic advection at speed `a` and time `t`.
/// (Valid while the pulse has not wrapped around the periodic boundary.)
std::vector<double> analytic_solution(const Grid2D& grid, double a_x, double a_y, double t);

}  // namespace advection
