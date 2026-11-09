#include <cstdlib>
#include <cstring>
#include <iostream>

#include "advection/config.hpp"
#include "advection/grid2d.hpp"
#include "advection/io.hpp"
#include "advection/problem.hpp"
#include "advection/solver.hpp"
#include "advection/flux.hpp"
#include "advection/spatial_scheme.hpp"
#include "advection/time_integrator.hpp"

int main(int argc, char** argv) {
    try {

        // read settings config file and parse
        const std::string config_path = (argc > 1) ? argv[1] : "config.toml";
        const advection::CaseConfig cfg = advection::load_config(config_path);

        // define grid
        // const advection::Grid1D grid(cfg.grid.x_left, cfg.grid.x_right, cfg.grid.n_x);
        const advection::Grid2D grid2d(cfg.grid.x_left, cfg.grid.x_right, cfg.grid.y_bottom, cfg.grid.y_top, cfg.grid.n_x, cfg.grid.n_y);

        // summarise case
        std::cout << "Case file : " << config_path << '\n'
                  << "Grid      : N = " << grid2d.size() << ", dx = " << grid2d.dx << ", dy = " << grid2d.dy << '\n'
                  << "Solver    : a_x = " << cfg.solver.a_x << ", CFL = " << cfg.solver.cfl
                  << ", t_end = " << cfg.solver.t_end << '\n';

        // initialise fields                  
        const auto u_initial = advection::initial_condition(grid2d);
        auto u = u_initial;

        auto time_integrator =  advection::make_integrator(cfg.solver.integrator);
        auto spatial_scheme = advection::make_scheme(cfg.solver.scheme, cfg.solver.limiter);

        // solve system
        const double t = advection::solve(u, grid2d.dx, cfg.solver, *time_integrator, *spatial_scheme);

        // generate analytic solution
        const auto u_exact = advection::analytic_solution(grid2d, cfg.solver.a_x, cfg.solver.a_y, t);


        // write to file for post processing
        advection::write_csv(cfg.output.outputfile, grid2d, u_initial, u, u_exact);
        std::cout << "Finished at t = " << t << std::endl;

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
