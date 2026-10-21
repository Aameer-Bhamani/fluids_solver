#pragma once

#include <string>
#include <vector>

#include "advection/grid.hpp"

namespace advection {

/// Write initial/final/analytic fields to a CSV file.
/// Throws std::runtime_error if the file cannot be opened.
void write_csv(const std::string& filename,
               const Grid1D& grid,
               const std::vector<double>& u_initial,
               const std::vector<double>& u_final,
               const std::vector<double>& u_analytic);

}  // namespace advection
