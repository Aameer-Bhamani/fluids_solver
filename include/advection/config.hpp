#pragma once

#include <string>
#include "advection/solver.hpp"

namespace advection {

struct GridConfig {
    double x_left = 0.0;
    double x_right = 1.0;
    int n_cells = 200;
};

struct OutputConfig
{
    std::string outputfile = "advection_results.csv";
};

struct CaseConfig {
    GridConfig grid;
    SolverConfig solver;
    OutputConfig output;
};

CaseConfig load_config(const std::string& path);

}
