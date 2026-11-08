#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <string>


namespace advection {
    using Limiter = double (*)(double, double);
    Limiter make_limiter(const std::string& name);

    class SpatialScheme {
        public:
            virtual ~SpatialScheme() = default;
            virtual int stencil_width() const = 0;
            virtual double face_flux (const double* u_left, double a) const = 0;

    };

    class Upwind : public SpatialScheme{
        public:
            int stencil_width() const override;
            double face_flux (const double* u_left, double a) const override;
    };

    class Central : public SpatialScheme{
        public:
            int stencil_width() const override;
            double face_flux (const double* u_left, double a) const override;
    };

    class MUSCL : public SpatialScheme{
        public:
            explicit MUSCL(Limiter limiter) : limiter_(limiter) {}
            int stencil_width() const override;
            double face_flux (const double* u_left, double a) const override;
        private:
            Limiter limiter_;
    };

    std::unique_ptr<SpatialScheme> make_scheme(const std::string& name, const std::string& limiter);

}  // namespace advection
