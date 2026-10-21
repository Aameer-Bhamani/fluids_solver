#pragma once

#include <vector>

namespace advection {

/// First-order upwind approximation of a * du/dx with periodic boundaries.
std::vector<double> upwind_divergence(const std::vector<double>& u, double a, double dx);

}  // namespace advection
