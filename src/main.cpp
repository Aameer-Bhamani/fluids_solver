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

        // read settings config file and parse
        const std::string config_path = (argc > 1) ? argv[1] : "config.toml";
        const advection::CaseConfig cfg = advection::load_config(config_path);

        // define grid
        const advection::Grid1D grid(cfg.grid.x_left, cfg.grid.x_right, cfg.grid.n_cells);

        // summarise case
        std::cout << "Case file : " << config_path << '\n'
                  << "Grid      : N = " << grid.n_cells << ", dx = " << grid.dx << '\n'
                  << "Solver    : a = " << cfg.solver.a << ", CFL = " << cfg.solver.cfl
                  << ", t_end = " << cfg.solver.t_end << '\n';

        // initialise fields                  
        const auto u_initial = advection::initial_condition(grid);
        auto u = u_initial;

        // solve system
        const double t = advection::solve(u, grid.dx, cfg.solver);

        // generate analytic solution
        const auto u_exact = advection::analytic_solution(grid, cfg.solver.a, t);


        // write to file for post processing
        advection::write_csv(cfg.output.outputfile, grid, u_initial, u, u_exact);
        std::cout << "Finished at t = " << t << std::endl;

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
