#pragma once

#include <vector>

#include "advection/spatial_scheme.hpp"

namespace advection {

/// First-order upwind approximation of a * du/dx with periodic boundaries.
std::vector<double> upwind_divergence(const std::vector<double>& u, double a, double dx);

std::vector<double> flux_divergence(const std::vector<double>& u, double a, double dx, const SpatialScheme& scheme);

// indices for periodic boundary conditions
std::vector<double> pad_periodic(const std::vector<double>& u, int ghost_cells);

}  // namespace advection
