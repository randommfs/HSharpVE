#include <ve/ve.hpp>

using HSharpVE::Variable;
using HSharp::VariableType;
using HSharpVE::VirtualEnvironment;

Variable& VirtualEnvironment::create_variable(std::string &name, VariableType vtype) {
    if (is_current_scope_global)
        return global_scope[name] = {.vtype = vtype};

    return (fscopes_top->back()[name] = {.vtype = vtype});
}

bool VirtualEnvironment::variable_exists(std::string &name) {
    if (global_scope.contains(name)) return true;
    if (function_scopes.empty()) return false;
    for (auto subscope : function_scopes.top())
        if (subscope.contains(name)) return true;

    return false;
}

void VirtualEnvironment::set_variable(std::string& name, void* value) {
    get_variable(name).value = value;
}

Variable& VirtualEnvironment::get_variable(std::string& name) {
    if (global_scope.contains(name))
        return global_scope[name];

    for (auto scope : *fscopes_top)
        if (scope.contains(name))
            return scope[name];
}

void VirtualEnvironment::create_scope() {
    function_scopes.push(FunctionScope{});
    fscopes_top = &function_scopes.top();
}

void VirtualEnvironment::destroy_scope() {
    fscopes_top->~vector<Scope>();
    function_scopes.pop();
    fscopes_top = &function_scopes.top();
}