#include "advection/io.hpp"

#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace advection {

void write_csv(const std::string& filename,
               const Grid1D& grid,
               const std::vector<double>& u_initial,
               const std::vector<double>& u_final,
               const std::vector<double>& u_analytic) {
    std::ofstream file(filename);
    if (!file) throw std::runtime_error("write_csv: cannot open " + filename);

    file << "initial,final,analytic,x,cell_volume\n";
    file << std::setprecision(16);
    for (int i = 0; i < grid.n_cells; ++i) {
        file << u_initial[i] << ',' << u_final[i] << ',' << u_analytic[i] << ','
             << grid.x[i] << ',' << grid.dx << '\n';
    }
}

}  // namespace advection
