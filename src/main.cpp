#include <cstdlib>
#include <cstring>
#include <iostream>

#include "advection/grid.hpp"
#include "advection/io.hpp"
#include "advection/problem.hpp"
#include "advection/solver.hpp"

int main(int argc, char** argv) {
    try {
        const bool plot = (argc > 1 && std::strcmp(argv[1], "--plot") == 0);

        const advection::Grid1D grid(0.0, 1.0, 20);
        advection::SolverConfig cfg;  // a = 1, CFL = 0.5, t_end = 0.4

        const auto u_initial = advection::initial_condition(grid);
        auto u = u_initial;

        const double t = advection::solve(u, grid.dx, cfg);
        const auto u_exact = advection::analytic_solution(grid, cfg.a, t);

        advection::write_csv("advection_results.csv", grid, u_initial, u, u_exact);
        std::cout << "Finished at t = " << t << ", results in advection_results.csv\n";

        if (plot) {
            // Run from the repository root so the script path resolves
            if (std::system("python3 scripts/post_process.py") != 0) {
                std::cerr << "Error: post_process.py failed\n";
                return 1;
            }
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
