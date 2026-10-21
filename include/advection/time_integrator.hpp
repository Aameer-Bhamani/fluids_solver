#pragma once

#include <vector>

namespace advection {

/// One forward Euler step: u^{n+1} = u^n - dt * a du/dx
std::vector<double> forward_euler_step(const std::vector<double>& u,
                                       double a, double dt, double dx);

}  // namespace advection
