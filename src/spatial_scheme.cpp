#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>

#include "advection/spatial_scheme.hpp"
#include "advection/flux.hpp"


double minmod(double a, double b) {
    if (a * b <= 0.0) return 0.0;
    return (std::abs(a) < std::abs(b)) ? a : b;
}

double van_leer(double a, double b) {
    if (a * b <= 0.0) return 0.0;
    return (2.0 * a * b) / (a + b);
}

namespace advection {

    int Upwind::stencil_width() const{return 1;}
    double Upwind::face_flux (const double* u_left, double a) const{
        double result = 0.0;
        result = (a >= 0.0) ? a * u_left[0] : a * u_left[1];
        return result;
    }

    int Central::stencil_width() const{return 1;}
    double Central::face_flux (const double* u_left, double a) const{
        double result = 0.0;
        result =  0.5 * a * (u_left[0] + u_left[1]);
        return result;
    }

    Limiter make_limiter(const std::string& name) {
        if (name == "minmod") {
            return minmod;
        } else if (name == "van_leer") {
            return van_leer;
        } else {
            throw std::runtime_error("Unknown limiter '" + name +
                                    "' (valid: minmod, van_leer)");
        }
    }

    int MUSCL::stencil_width() const{return 2;}
    double MUSCL::face_flux (const double* u_left, double a) const{
        double result = 0.0;

        double du_m1 = u_left[0] - u_left[-1];
        double du_0 = u_left[1] - u_left[0];
        double du_p1 = u_left[2] - u_left[1];

        double slope_l = limiter_(du_m1, du_0);
        double slope_r = limiter_(du_0, du_p1);

        double u_L = u_left[0] + 0.5 * slope_l;
        double u_R = u_left[1] - 0.5 * slope_r;


        result = (a >= 0.0) ? a * u_L : a * u_R;
        return result;
    }
    

    std::unique_ptr<SpatialScheme> make_scheme(const std::string& name, const std::string& limiter){
        std::unique_ptr<SpatialScheme> method;

        if (name == "upwind"){
            method = std::make_unique<Upwind>();
        } else if (name == "central"){
            method= std::make_unique<Central>();
        } else if (name == "muscl") {
            method = std::make_unique<MUSCL>(make_limiter(limiter));
        } else{
            throw std::runtime_error("Unknown Scheme '" + name +
                                    "' (valid: Upwind, Central, MUSCL)");
        }
        
        return method;
    }

} // namespace advection
