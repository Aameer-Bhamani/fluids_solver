#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

#include "advection/spatial_operator.hpp"
#include "advection/flux.hpp"

namespace advection {

std::vector<double> AdvectionOperator::apply(const std::vector<double>& u) const 
{ 
    if (a_x_ == 0.0 && a_y_ == 0.0) throw std::invalid_argument("solve: advection speed must be non-zero");
    // Implementation for applying advection operator
    std::vector<double> div(grid_.size(), 0.0);

    for (int j = 0; j < grid_.n_y; ++j) {            // x sweep: one row at a time
        std::vector<double> row(u.begin() + j*grid_.n_x, u.begin() + (j+1)*grid_.n_x);
        std::vector<double> row_div = flux_divergence(row, a_x_, grid_.dx, scheme_);
        for (int i = 0; i < grid_.n_x; ++i)
            div[grid_.idx(i, j)] += row_div[i];
    }

    if (grid_.n_y > 1) {                             // y sweep: skip for 1D runs
        for (int i = 0; i < grid_.n_x; ++i) {
            std::vector<double> col(grid_.n_y);
            for (int j = 0; j < grid_.n_y; ++j) col[j] = u[grid_.idx(i, j)];
            std::vector<double> col_div = flux_divergence(col, a_y_, grid_.dy, scheme_);
            for (int j = 0; j < grid_.n_y; ++j)
                div[grid_.idx(i, j)] += col_div[j];
        }
    }

    for (double& val : div) {
        val *= -1;  // Negate the divergence to match the advection equation form
    }

    return div;
}

double AdvectionOperator::stable_dt() const 
{
    return 1.0 / (fabs(a_x_) / grid_.dx + fabs(a_y_) / grid_.dy);
}

AdvectionOperator::AdvectionOperator(const Grid2D& grid, double a_x, double a_y, const SpatialScheme& scheme)
    : grid_(grid), a_x_(a_x), a_y_(a_y), scheme_(scheme) {}

std::unique_ptr<SpatialOperator> make_operator(
    const Grid2D& grid,
    double a_x,
    double a_y,
    const SpatialScheme& scheme
) {
    return std::make_unique<AdvectionOperator>(
        grid, a_x, a_y, scheme
    );
}

} // namespace advection
