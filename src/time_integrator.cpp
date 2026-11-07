#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

#include "advection/time_integrator.hpp"
#include "advection/flux.hpp"

namespace advection {

std::vector<double> ForwardEuler::step(const std::vector<double> &u, double dt, const RHSFunction &rhs) const 
{ 
    std::vector<double> result(u.size());
    std::vector<double> flux = rhs(u);
    for (std::size_t i = 0; i < u.size(); ++i) result[i] = u[i] + dt * flux[i];
    return result;
}

std::vector<double> Heun::step(const std::vector<double> &u, double dt, const RHSFunction &rhs) const
{
    int mnode = u.size();
    std::vector<double> result(mnode), inter_val(mnode);
    std::vector<double> firstFlux = rhs(u);
    for (int i = 0; i < mnode; ++i) inter_val[i] = u[i] + dt * firstFlux[i];
    std::vector<double> secondFlux = rhs(inter_val);
    for (int i = 0; i < mnode; ++i) result[i] = u[i] + 0.5 * dt * (secondFlux[i] + firstFlux[i]);

    return result;
}

std::unique_ptr<TimeIntegrator> make_integrator(const std::string &name)
{
    std::unique_ptr<TimeIntegrator> method;

    if (name == "forward_euler"){
        method = std::make_unique<ForwardEuler>();
    } else if (name == "heun"){
        method= std::make_unique<Heun>();
    } else {
    throw std::runtime_error("Unknown integrator '" + name +
                             "' (valid: forward_euler, heun)");
    }

    return method;
}

} // namespace advection
