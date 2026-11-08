# Fluids Solver

A 1D linear advection solver written in C++17,

    u_t + a u_x = 0

on a periodic domain, using a cell-centred finite volume method. The test case is a Gaussian pulse that is compared against the analytic solution (the initial pulse shifted by `a*t`). The whole case is described by a TOML file, and the spatial scheme, slope limiter and time integrator are all selectable from it.

## Features

- Config-driven: grid, solver and output settings come from a TOML file
- Spatial schemes: first-order upwind, second-order central, MUSCL
- Slope limiters for MUSCL: minmod, van Leer
- Time integrators: forward Euler, Heun (RK2)
- Periodic boundaries handled with ghost cells, so schemes of any stencil width plug in
- Last time step is clipped so the solution lands exactly on `t_end`
- CSV output and a Python script for plotting

## Requirements

- C++17 compiler and CMake (3.14 or newer recommended)
- Internet access on the first configure: [toml++](https://github.com/marzer/tomlplusplus) v3.4.0 is fetched with `FetchContent`
- Python 3 with `numpy` and `matplotlib` for `scripts/post_process.py` (TODO: confirm the exact package list)

## Build and run

    cmake -S . -B build
    cmake --build build
    ./build/advection_solver config/case.toml

The config path is the first command-line argument. If it is omitted, `config.toml` in the current directory is used.

Relative paths (the config file and the output CSV) are resolved against the **current working directory**, not the executable's folder. If the solver cannot find the config, or the results look stale, check `pwd` and `ls` first. Running from the repository root is the simplest option.

To plot the results:

    python3 scripts/post_process.py

(TODO: confirm the exact arguments, and whether `--plot` on the solver still works.)

## Configuration

Example `config/case.toml` (the values here are illustrative, see the file in the repository for the real defaults):

    [grid]
    # domain and number of cells

    [solver]
    integrator = "hen"        # forward_euler | heun 
    scheme     = "muscl"      # upwind | central | muscl
    limiter    = "van_leer"   # minmod | van_leer (only used by muscl)

    [initial_condition]
    # Gaussian pulse parameters

    [output]
    # output file name

| Key | Valid values | Notes |
|---|---|---|
| `solver.integrator` | `forward_euler`, `heun`, `rk4` | Unknown names throw an error listing the valid ones |
| `solver.scheme` | `upwind`, `central`, `muscl` | Unknown names throw an error listing the valid ones |
| `solver.limiter` | `minmod`, `van_leer` | Only used when `scheme = "muscl"`; ignored otherwise |

## Numerical methods

The semi-discrete form is `du/dt = L(u)` with `L(u) = -a du/dx`. The spatial scheme builds `L(u)` and the time integrator advances it, and neither knows about the other.

### Spatial schemes

Each scheme computes a flux `F` at every cell face from the cells around it. The cell update is

    du_i/dt = -(F_{i+1/2} - F_{i-1/2}) / dx

With `uL` and `uR` the cell values on the left and right of a face:

| Scheme | Face flux | Formal order | Ghost cells per side |
|---|---|---|---|
| `upwind` | `a*uL` if `a >= 0`, else `a*uR` | 1 | 1 |
| `central` | `a*(uL + uR)/2` | 2 | 1 |
| `muscl` | upwind flux of the limited, reconstructed face states | up to 2 | 2 |

**MUSCL.** Each cell gets a limited slope `s`. The face states are `uL = u_i + s_i/2` and `uR = u_{i+1} - s_{i+1}/2`, and the flux is upwinded on those. With `p = u_i - u_{i-1}` and `q = u_{i+1} - u_i`:

| Limiter | Slope |
|---|---|
| `minmod` | `0` if `p*q <= 0`, otherwise whichever of `p`, `q` has the smaller magnitude |
| `van_leer` | `0` if `p*q <= 0`, otherwise `2pq / (p + q)` |

Both limiters fall back to a zero slope (first order) at local extrema, so the observed order on a smooth pulse is somewhat below 2.

### Time integrators

| Name | Scheme | Order |
|---|---|---|
| `forward_euler` | `u + dt*k1` | 1 |
| `heun` | `u + dt/2 (k1 + k2)` | 2 |
| `rk4` | classical four-stage Runge-Kutta | 4 |

The time step is `dt = cfl * dx / |a|`, clipped on the last step.

## Known behaviour

- `central` with `forward_euler` is unstable and blows up. This is expected, not a bug. Use `heun` or `rk4` with `central`.
- Stability limits differ between integrators, so raise `cfl` gradually to find where each combination fails.
- With upwind, the O(dx) spatial error hides the time-integration error, so switching integrators barely changes the error on the same mesh. To measure temporal order, use a fixed mesh and compare against a tiny-`dt` reference run.
- At CFL = 1, forward Euler with upwind is an exact shift of the solution. Heun and RK4 are not.
- Unknown keys in the TOML file are silently ignored.

## Results

Observed spatial order of accuracy (TODO: fill in with your measured numbers; state the integrator and `dt` used).

| Scheme | Limiter | Observed order |
|---|---|---|
| upwind | - | TODO |
| central | - | TODO |
| muscl | minmod | TODO |
| muscl | van_leer | TODO |

## Project layout

    config/case.toml         example case
    include/advection/       public headers
    src/                     implementations and main.cpp
    scripts/post_process.py  plotting script

Main components:

- `time_integrator.hpp`: abstract `TimeIntegrator` with `ForwardEuler`, `Heun`, `RK4` and a `make_integrator` factory
- `spatial_scheme.hpp`: abstract `SpatialScheme` (face flux and stencil width) with `Upwind`, `Central`, `MUSCL`, plus `make_scheme` and `make_limiter`
- `flux.hpp`: periodic ghost-cell padding and the flux divergence operator
- `solver.hpp`: the time loop
- `config.hpp`: TOML loading and validation

## Roadmap

- Unit and convergence tests, and CI
- Extension to 2D
