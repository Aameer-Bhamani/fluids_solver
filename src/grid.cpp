#include "advection/grid.hpp"

#include <stdexcept>

namespace advection {

Grid1D::Grid1D(double xl, double xr, int n)
    : x_left(xl), x_right(xr), n_cells(n), dx(0.0), x() {
    if (n <= 0)   throw std::invalid_argument("Grid1D: n_cells must be positive");
    if (xr <= xl) throw std::invalid_argument("Grid1D: x_right must exceed x_left");

    dx = (x_right - x_left) / static_cast<double>(n_cells);
    x.resize(n_cells);
    for (int i = 0; i < n_cells; ++i) {
        x[i] = x_left + (static_cast<double>(i) + 0.5) * dx;
    }
}

}  // namespace advection
