#pragma once

#include <vector>

namespace advection {

/// Uniform 2D cell-centred finite volume grid.
struct Grid2D {
    double x_left;
    double x_right;
    double y_bottom;
    double y_top;
    int    n_x;
    int    n_y;
    double dx;
    double dy;
    std::vector<double> x;  ///< cell-centre coordinates
    std::vector<double> y;  ///< cell-centre coordinates

    int size() const { return n_x * n_y; }
    int idx(int i, int j) const { return i + n_x * j; }

    Grid2D(double x_left, double x_right, double y_bottom, double y_top, int n_x, int n_y);
};

}  // namespace advection
