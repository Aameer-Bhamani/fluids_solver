#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <string>


namespace advection {
using RHSFunction = std::function<std::vector<double>(const std::vector<double>&)>;

class TimeIntegrator {
    public:
        virtual ~TimeIntegrator() = default;
        virtual std::vector<double> step(const std::vector<double>& u, 
                                        double dt,
                                        const RHSFunction& rhs) const = 0;

};

class ForwardEuler : public TimeIntegrator{
    public:
        std::vector<double> step(const std::vector<double>& u, double dt,
                                const RHSFunction& rhs) const override;
};

class Heun : public TimeIntegrator{
    public:
    std::vector<double> step(const std::vector<double>& u, double dt,
                             const RHSFunction& rhs) const override;
};

// class RK4 : public TimeIntegrator{
//     public:
//     std::vector<double> step(const std::vector<double>& u, double dt,
//                              const RHSFunction& rhs) const override;
// };

std::unique_ptr<TimeIntegrator> make_integrator(const std::string& name);

}  // namespace advection
