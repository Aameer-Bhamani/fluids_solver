# Fluids Solver

A linear advection solver in C++17 for one and two dimensions,

    u_t + a_x u_x + a_y u_y = 0      (2D)
    u_t + a_x u_x = 0                (1D)

on a periodic domain, using a cell-centred finite volume method. The test case is a Gaussian pulse that is compared against the analytic solution (the initial pulse shifted by `a*t` and wrapped periodically). The whole case is described by a TOML file: the dimension, the spatial scheme, the slope limiter and the time integrator are all selectable there.

## Features

- 1D and 2D, selected with `mdim` in the config file
- Config-driven: grid, solver and output settings come from a TOML file, with validation and readable error messages
- Spatial schemes: first-order upwind, second-order central, MUSCL
- Slope limiters for MUSCL: minmod, van Leer
- Time integrators: forward Euler, Heun (RK2), classical RK4
- Periodic boundaries handled with ghost cells, so schemes of any stencil width plug in
- Spatial operators behind a common interface (`SpatialOperator`), so other operators (for example diffusion) can be added later
- The last time step is clipped so the solution lands exactly on `t_end`
- CSV output and a Python script for plotting

## Requirements

- C++17 compiler and CMake (TODO: confirm the minimum version in `CMakeLists.txt`)
- Internet access on the first configure: [toml++](https://github.com/marzer/tomlplusplus) v3.4.0 is fetched with `FetchContent`
- Python 3 with `numpy` and `matplotlib` for `scripts/post_process.py` (TODO: confirm the package list)

## Build and run

    cmake -S . -B build
    cmake --build build
    ./build/advection_solver config/case.toml

The config path is the first command-line argument. If it is omitted, `config.toml` in the current directory is used. The dimension comes from `mdim` in the config file.

Relative paths (the config file and the output CSV) are resolved against the **current working directory**, not the executable's folder. If the solver cannot find the config, or the results look stale, check `pwd` and `ls` first.

To plot the results:

    python3 scripts/post_process.py

(TODO: confirm the exact arguments, and how the plot script handles 1D and 2D files.)

## Configuration

Example `config/case.toml` (check the file in the repository for the real defaults):

    [grid]
    mdim     = 2          # 1 or 2
    x_left   = 0.0
    x_right  = 1.0
    n_x      = 200
    y_bottom = 0.0        # 2D only
    y_top    = 1.0        # 2D only
    n_y      = 200        # 2D only

    [solver]
    a_x        = 1.0
    a_y        = 1.0      # 2D only
    cfl        = 0.2
    t_end      = 0.4
    integrator = "rk4"        # forward_euler | heun | rk4
    scheme     = "muscl"      # upwind | central | muscl
    limiter    = "van_leer"   # minmod | van_leer (only used by muscl)

    [output]
    csv_file = "advection_results.csv"

| Key | Valid values | Notes |
|---|---|---|
| `grid.mdim` | `1`, `2` | Any other value throws an error |
| `grid.x_left`, `grid.x_right` | numbers | `x_right` must exceed `x_left` |
| `grid.n_x` | positive integer | Number of cells in x |
| `grid.y_bottom`, `grid.y_top`, `grid.n_y` | numbers | Read only when `mdim = 2`. `y_top` must exceed `y_bottom` |
| `solver.a_x` | number | Advection speed in x. Must be non-zero in 1D |
| `solver.a_y` | number | Read only when `mdim = 2`. `a_x` and `a_y` must not both be zero |
| `solver.cfl` | positive number | Multiplies the largest stable time step |
| `solver.t_end` | non-negative number | Final time |
| `solver.integrator` | `forward_euler`, `heun`, `rk4` | Unknown names throw an error listing the valid ones |
| `solver.scheme` | `upwind`, `central`, `muscl` | Unknown names throw an error listing the valid ones |
| `solver.limiter` | `minmod`, `van_leer` | Only used when `scheme = "muscl"` |
| `output.csv_file` | file name | Relative to the working directory |

In a 1D run, any 2D keys in the file are ignored and keep their defaults, so a 2D config can be reused for 1D by changing `mdim`.

The initial condition is currently fixed in the code: `u = 1 + 0.5 exp(-100 r^2)`, a Gaussian bump centred at 0.3 (in both x and y in 2D) on a constant background of 1. Making it configurable is on the roadmap.

## Numerical methods

The semi-discrete form is `du/dt = L(u)`. A `SpatialOperator` builds `L(u)` (sign included) and a `TimeIntegrator` advances it. Neither knows about the other, and the integrators only see a vector.

### Spatial schemes

Each scheme computes a flux `F` at a cell face from the cells around it. With `uL` and `uR` the cell values on the left and right of a face:

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

Both limiters give a zero slope (first order) at local extrema, so the observed order on a smooth pulse is somewhat below 2.

### How 2D works

The scheme is applied **dimension by dimension**. In 1D, the update is

    du_i/dt = -(F_{i+1/2} - F_{i-1/2}) / dx

and in 2D the x and y contributions are added:

    du_ij/dt = -(Fx_{i+1/2,j} - Fx_{i-1/2,j}) / dx - (Fy_{i,j+1/2} - Fy_{i,j-1/2}) / dy

The `AdvectionOperator` sweeps along every row with `a_x` and `dx`, then along every column with `a_y` and `dy`, reusing the same 1D face-flux schemes in both directions. The field is stored as a flat vector with `idx(i, j) = i + n_x * j` (`i` is the x index, `j` is the y index). In a 1D run (`n_y = 1`) the y sweep is skipped.

### Time integrators

| Name | Scheme | Order |
|---|---|---|
| `forward_euler` | `u + dt*k1` | 1 |
| `heun` | `u + dt/2 (k1 + k2)` | 2 |
| `rk4` | classical four-stage Runge-Kutta | 4 |

### Time step

The time step is `dt = cfl * dt_stable`, where the operator reports its own stable step:

- 2D: `dt_stable = 1 / (|a_x|/dx + |a_y|/dy)`
- 1D: `dt_stable = dx / |a_x|`

The last step is clipped so that the solution lands exactly on `t_end`.

## Output

The solver writes a CSV with the columns

    initial,final,analytic,x,y,cell_volume

with one row per cell, written row by row (x varies fastest). `cell_volume` is `dx*dy`. (TODO: confirm what a 1D run writes.) The solver also prints the L_inf and L2 errors against the analytic solution.

## Known behaviour

- `central` with `forward_euler` is unstable and blows up. This is expected, not a bug. Use `heun` or `rk4` with `central`.
- Stability limits differ between integrators and schemes, so raise `cfl` gradually to find where each combination fails.
- With upwind, the O(dx) spatial error hides the time-integration error, so switching integrators barely changes the error on the same mesh. To measure temporal order, use a fixed fine mesh and compare against a tiny-`dt` reference run.
- At CFL = 1 in 1D, forward Euler with upwind is an exact shift of the solution. Heun and RK4 are not.
- The analytic solution assumes the pulse is narrow compared to the domain (it wraps the position into the domain once).
- Unknown keys in the TOML file are silently ignored.

## Results

Observed spatial order of accuracy. TODO: fill in with your measured numbers, and state the integrator, `dt` or CFL, norm and mesh sequence (for example N = 50, 100, 200, 400). A preliminary check with `heun` and `central` on a 2D grid, refining from `dx = 0.05` to `dx = 0.005` at CFL = 0.01, gave an order of roughly 1.7 (L_inf) to 1.8 (L2). The coarse mesh is under-resolved, so a finer sequence is needed before quoting an order.

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

- `grid2d.hpp`: uniform cell-centred grid with `idx(i, j)`, `dx`, `dy` and cell-centre coordinates
- `problem.hpp`: initial condition and analytic solution (periodic, 1D and 2D)
- `spatial_scheme.hpp`: abstract `SpatialScheme` (face flux and stencil width) with `Upwind`, `Central`, `MUSCL`, plus `make_scheme` and `make_limiter`
- `flux.hpp`: periodic ghost-cell padding and the 1D flux divergence
- `spatial_operator.hpp`: abstract `SpatialOperator` (`apply` returns `du/dt`, `stable_dt` returns the stable step) and `AdvectionOperator`
- `time_integrator.hpp`: abstract `TimeIntegrator` with `ForwardEuler`, `Heun`, `RK4` and a `make_integrator` factory
- `solver.hpp`: the time loop
- `config.hpp`: TOML loading and validation
- `io.hpp`: CSV output

## Roadmap

- Unit and convergence tests (`tests/`) and CI
- Configurable initial condition
- Diffusive operators, using the existing `SpatialOperator` interface
- Parallelisation with MPI (domain decomposition with halo exchange)
