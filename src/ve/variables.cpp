#include <ve/ve.hpp>

using HSharpVE::Variable;
using HSharp::VariableType;
using HSharpVE::VirtualEnvironment;

Variable& VirtualEnvironment::create_variable(std::string &name, VariableType vtype) {
    if (is_current_scope_global)
        return global_scope[name] = {.vtype = vtype};

    return (function_scopes.top().back()[name] = {.vtype = vtype});
}

bool VirtualEnvironment::variable_exists(std::string &name) {
    if (global_scope.contains(name)) return true;
    for (auto subscope : function_scopes.top())
        if (subscope.contains(name)) return true;

    return false;
}