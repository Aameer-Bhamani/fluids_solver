#include "advection/config.hpp"

#include <stdexcept>
#include <string>
#include <string_view>

#include <third_party/toml.hpp>

namespace advection {

namespace {

// Read tbl[section][key]. Returns `def` if absent, throws if present but the wrong type.
template <typename T>
T get_or(const toml::table& tbl, std::string_view section, std::string_view key, T def) {
    const auto node = tbl[section][key];
    if (!node) return def;
    if (const auto v = node.template value<T>()) return *v;
    throw std::runtime_error("config: [" + std::string(section) + "] " +
                             std::string(key) + " has the wrong type");
}

void validate(const CaseConfig& c) {
    if (c.grid.n_cells <= 0)
        throw std::runtime_error("config: [grid] n_cells must be positive");
    if (c.grid.x_right <= c.grid.x_left)
        throw std::runtime_error("config: [grid] x_right must exceed x_left");
    if (c.solver.a == 0.0)
        throw std::runtime_error("config: [solver] a must be non-zero");
    if (c.solver.cfl <= 0.0)
        throw std::runtime_error("config: [solver] cfl must be positive");
    if (c.solver.t_end < 0.0)
        throw std::runtime_error("config: [solver] t_end must be non-negative");
}

}  // namespace

CaseConfig load_config(const std::string& path) {
    toml::table tbl;
    try {
        tbl = toml::parse_file(path);
    } catch (const toml::parse_error& e) {
        throw std::runtime_error("config: cannot parse '" + path + "': " +
                                 std::string(e.description()));
    }

    CaseConfig c;  // starts with defaults

    c.grid.x_left  = get_or(tbl, "grid", "x_left",  c.grid.x_left);
    c.grid.x_right = get_or(tbl, "grid", "x_right", c.grid.x_right);
    c.grid.n_cells = get_or(tbl, "grid", "n_cells", c.grid.n_cells);

    c.solver.a     = get_or(tbl, "solver", "a",     c.solver.a);
    c.solver.cfl   = get_or(tbl, "solver", "cfl",   c.solver.cfl);
    c.solver.t_end = get_or(tbl, "solver", "t_end", c.solver.t_end);

    c.output.outputfile   = get_or(tbl, "output", "csv_file",    c.output.outputfile);

    validate(c);
    return c;
}

}  // namespace advection