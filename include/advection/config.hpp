#pragma once

#include <string>
#include "advection/solver.hpp"

namespace advection {

    // define grid values, assign default values
    struct GridConfig {
        int mdim = 1;
        double x_left = 0.0;
        double x_right = 1.0;
        double y_bottom = 0.0;
        double y_top = 1.0;
        int n_x = 200;
        int n_y = 1;
    };

    // define output path, assign default values
    struct OutputConfig
    {
        std::string outputfile = "results.csv";
    };

    struct CaseConfig {
        GridConfig grid;
        SolverConfig solver;
        OutputConfig output;
    };

    CaseConfig load_config(const std::string& path);

}
