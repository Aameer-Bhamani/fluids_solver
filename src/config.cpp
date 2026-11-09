#include "advection/config.hpp"

#include <stdexcept>
#include <string>
#include <string_view>

#include <third_party/toml.hpp>

namespace advection {

namespace {

// Read tbl[section][key]. Handles exceptions in case file is missing or key does not exist
template <typename T>
T get_or(const toml::table& tbl, std::string_view section, std::string_view key, T def) {
    const auto node = tbl[section][key];
    if (!node) return def;
    if (const auto v = node.template value<T>()) return *v;
    throw std::runtime_error("config: [" + std::string(section) + "] " +
                             std::string(key) + " has the wrong type");
}

// validate settings are sound
void validate(const CaseConfig& c) {
    if (c.grid.mdim == 1){
        if (c.grid.n_x <= 0)
            throw std::runtime_error("config: [grid] n_x must be positive");
        if (c.grid.x_right <= c.grid.x_left)
            throw std::runtime_error("config: [grid] x_right must exceed x_left");
        if (c.solver.a_x == 0.0)
            throw std::runtime_error("config: [solver] a_x must not be zero");
    } else if (c.grid.mdim == 2){
        if (c.grid.n_x <= 0 || c.grid.n_y <= 0)
            throw std::runtime_error("config: [grid] n_x and n_y must be positive");
        if (c.grid.x_right <= c.grid.x_left)
            throw std::runtime_error("config: [grid] x_right must exceed x_left");
        if (c.grid.y_top <= c.grid.y_bottom)
            throw std::runtime_error("config: [grid] y_top must exceed y_bottom");
        if (c.solver.a_x == 0.0 && c.solver.a_y == 0.0)
            throw std::runtime_error("config: [solver] a_x and a_y must not both be zero");
    } else {
        throw std::runtime_error("config: [grid] mdim must be 1 or 2");
    }


    if (c.solver.cfl <= 0.0)
        throw std::runtime_error("config: [solver] cfl must be positive");
    if (c.solver.t_end < 0.0)
        throw std::runtime_error("config: [solver] t_end must be non-negative");

}

}  // namespace

// extract case setup
CaseConfig load_config(const std::string& path) {
    toml::table tbl;
    try {
        tbl = toml::parse_file(path);
    } catch (const toml::parse_error& e) {
        throw std::runtime_error("config: cannot parse '" + path + "': " +
                                 std::string(e.description()));
    }

    CaseConfig c;  // starts with defaults

    // read toml for config values, or use defaults if not present
    
    c.grid.mdim = get_or(tbl, "grid", "mdim", c.grid.mdim);

    if (c.grid.mdim != 1 && c.grid.mdim != 2)
        throw std::runtime_error("config: [grid] mdim must be 1 or 2");

    c.grid.x_left  = get_or(tbl, "grid", "x_left",  c.grid.x_left);
    c.grid.x_right = get_or(tbl, "grid", "x_right", c.grid.x_right);
    c.grid.n_x = get_or(tbl, "grid", "n_x", c.grid.n_x);

    c.solver.a_x     = get_or(tbl, "solver", "a_x",     c.solver.a_x);
    c.solver.cfl   = get_or(tbl, "solver", "cfl",   c.solver.cfl);
    c.solver.t_end = get_or(tbl, "solver", "t_end", c.solver.t_end);
    c.solver.integrator = get_or(tbl, "solver", "integrator", c.solver.integrator);
    c.solver.scheme = get_or(tbl, "solver", "scheme", c.solver.scheme);
    c.solver.limiter = get_or(tbl, "solver", "limiter", c.solver.limiter);
    c.output.outputfile   = get_or(tbl, "output", "csv_file",    c.output.outputfile);

    if (c.grid.mdim == 2){
        c.grid.y_bottom = get_or(tbl, "grid", "y_bottom", c.grid.y_bottom);
        c.grid.y_top = get_or(tbl, "grid", "y_top", c.grid.y_top);
        c.grid.n_y = get_or(tbl, "grid", "n_y", c.grid.n_y);
        c.solver.a_y     = get_or(tbl, "solver", "a_y",     c.solver.a_y);
    }


    validate(c);
    return c;
}

}  // namespace advection