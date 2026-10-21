# Fluids Solver

Currently has functionality to solve 1D linear advection with a cell-centred
finite volume (first-order upwind in space, forward Euler in time, periodic boundaries).

## Layout

    include/advection/   public headers
    src/                 implementations + main.cpp

## Build & run

    cmake -S . -B build
    cmake --build build
    ./build/advection_solver           # writes advection_results.csv
    ./build/advection_solver --plot    # also runs scripts/post_process.py

Run from the repository root.
