#pragma once

#include <vector>

namespace advection {

/// Uniform 1D cell-centred finite volume grid.
struct Grid1D {
    double x_left;
    double x_right;
    int    n_cells;
    double dx;
    std::vector<double> x;  ///< cell-centre coordinates

    Grid1D(double x_left, double x_right, int n_cells);
};

}  // namespace advection
