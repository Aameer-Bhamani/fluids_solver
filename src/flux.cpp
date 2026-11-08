#include "advection/flux.hpp"
#include "advection/spatial_scheme.hpp"

#include <vector>
#include <algorithm>

namespace advection {


    std::vector<double> flux_divergence(const std::vector<double>& u, double a, double dx, const SpatialScheme& scheme) {
        const int n = u.size();
        std::vector<double> result(n), flux(n + 1);

        // Determine the number of ghost cells needed based on the stencil width
        const int ghost_cells = scheme.stencil_width();

        // Pad the input array with periodic boundary conditions
        std::vector<double> padded_u = pad_periodic(u, ghost_cells);

        // Compute the flux divergence using the specified spatial scheme
        // loop over faces
        for (int i = 0; i < n + 1; ++i) {
            flux[i] = scheme.face_flux(&padded_u[ghost_cells + i  - 1], a);
        }

        for (int i = 0; i < n; ++i) result[i] = (flux[i+1] - flux[i]) / dx;

        return result;
    }

    std::vector<double> pad_periodic(const std::vector<double>& u, int ghost_cells) {
        const int n = static_cast<int>(u.size());
        std::vector<double> padded_u(n + 2 * ghost_cells);

        // Copy the original data into the center of the padded array
        std::copy(u.begin(), u.end(), padded_u.begin() + ghost_cells);

        // Fill the ghost cells with periodic boundary conditions
        for (int i = 0; i < ghost_cells; ++i) {
            padded_u[i] = u[n - ghost_cells + i];  // Left ghost cells
            padded_u[n + ghost_cells + i] = u[i];  // Right ghost cells
        }

        return padded_u;
    }

}  // namespace advection
