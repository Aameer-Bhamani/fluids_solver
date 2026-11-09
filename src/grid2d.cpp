#include "advection/grid2d.hpp"

#include <stdexcept>

namespace advection {

Grid2D::Grid2D(double xl, double xr, double yb, double yt, int n_x, int n_y)
    : x_left(xl), x_right(xr), y_bottom(yb), y_top(yt), n_x(n_x), n_y(n_y), dx(0.0), dy(0.0), x(), y() {
    if (n_x <= 0 || n_y <= 0)
        throw std::invalid_argument("Grid2D: n_x and n_y must be positive");
    if (xr <= xl || yt <= yb)
        throw std::invalid_argument("Grid2D: x_right must exceed x_left and y_top must exceed y_bottom");

    dx = (x_right - x_left) / static_cast<double>(n_x);
    dy = (y_top - y_bottom) / static_cast<double>(n_y);
    x.resize(n_x);
    y.resize(n_y);
    for (int i = 0; i < n_x; ++i) {
        x[i] = x_left + (static_cast<double>(i) + 0.5) * dx;
    }
    for (int j = 0; j < n_y; ++j) {
        y[j] = y_bottom + (static_cast<double>(j) + 0.5) * dy;
    }
}

}  // namespace advection
