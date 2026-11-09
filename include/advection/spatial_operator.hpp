#pragma once

#include <vector>

#include "advection/grid2d.hpp"
#include "advection/spatial_scheme.hpp"

namespace advection {

    class SpatialOperator {
    public:
        virtual ~SpatialOperator() = default;
        virtual std::vector<double> apply(const std::vector<double>& u) const = 0;
        virtual double stable_dt() const = 0;
    };

    class AdvectionOperator : public SpatialOperator {
    public:
        AdvectionOperator(const Grid2D& grid, double a_x, double a_y, const SpatialScheme& scheme);
        std::vector<double> apply(const std::vector<double>& u) const override;
        double stable_dt() const override;

    private:
        const Grid2D& grid_;
        double a_x_;
        double a_y_;
        const SpatialScheme& scheme_;
    };   

    std::unique_ptr<SpatialOperator> make_operator(
    const Grid2D& grid,
    double a_x,
    double a_y,
    const SpatialScheme& scheme
);

}