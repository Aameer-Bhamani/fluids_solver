# advection1d

1D linear advection solved with a cell-centred finite volume method
(first-order upwind in space, forward Euler in time, periodic boundaries).

## Layout

    include/advection/   public headers
    src/                 implementations + main.cpp
    scripts/             post-processing (put post_process.py here)

## Build & run

    cmake -S . -B build
    cmake --build build
    ./build/advection_solver           # writes advection_results.csv
    ./build/advection_solver --plot    # also runs scripts/post_process.py

Run from the repository root.
