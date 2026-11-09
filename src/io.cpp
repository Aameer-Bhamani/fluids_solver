#include "advection/io.hpp"

#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace advection {

void write_csv(const std::string& filename,
               const Grid2D& grid,
               const std::vector<double>& u_initial,
               const std::vector<double>& u_final,
               const std::vector<double>& u_analytic) {
    std::ofstream file(filename);
    if (!file) throw std::runtime_error("write_csv: cannot open " + filename);

    const double cell_volume = grid.dx * grid.dy;

    file << "initial,final,analytic,x,y,cell_volume\n";
    file << std::setprecision(16);
    for (int j = 0; j < grid.n_y; ++j) {
        for (int i = 0; i < grid.n_x; ++i) {
            const int k = grid.idx(i, j);
            file << u_initial[k] << ',' << u_final[k] << ',' << u_analytic[k] << ','
                 << grid.x[i] << ',' << grid.y[j] << ',' << cell_volume << '\n';
        }
    }
}

}  // namespace advection