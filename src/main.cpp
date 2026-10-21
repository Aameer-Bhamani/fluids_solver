#include <cstdlib>
#include <cstring>
#include <iostream>

#include "advection/config.hpp"
#include "advection/grid.hpp"
#include "advection/io.hpp"
#include "advection/problem.hpp"
#include "advection/solver.hpp"

int main(int argc, char** argv) {
    try {
        const std::string config_path = (argc > 1) ? argv[1] : "case_settings.toml";
        // const std::string config_path = "config/case.toml";
        const advection::CaseConfig cfg = advection::load_config(config_path);

        const advection::Grid1D grid(cfg.grid.x_left, cfg.grid.x_right, cfg.grid.n_cells);

        std::cout << "Case file : " << config_path << '\n'
                  << "Grid      : N = " << grid.n_cells << ", dx = " << grid.dx << '\n'
                  << "Solver    : a = " << cfg.solver.a << ", CFL = " << cfg.solver.cfl
                  << ", t_end = " << cfg.solver.t_end << '\n';

        const auto u_initial = advection::initial_condition(grid);
        auto u = u_initial;

        const double t = advection::solve(u, grid.dx, cfg.solver);
        const auto u_exact = advection::analytic_solution(grid, cfg.solver.a, t);

        advection::write_csv(cfg.output.outputfile, grid, u_initial, u, u_exact);
        std::cout << "Finished at t = " << t << std::endl;

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
