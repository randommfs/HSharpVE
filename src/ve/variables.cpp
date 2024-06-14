#include <ve/ve.hpp>

using HSharpVE::Variable;
using HSharp::VariableType;
using HSharpVE::VirtualEnvironment;

Variable& VirtualEnvironment::create_variable(std::string &name, VariableType vtype) {
    std::cout << global_scopes.size() << std::endl;
    if (is_current_scope_global){
        Variable& ref = global_scopes.back()[name];
        ref = {.vtype = vtype};
        return ref;
    }
    
    std::cout << fscopes_top->size() << std::endl;
    std::terminate();
    Variable& var = fscopes_top->back()[name];
    var = {.vtype = vtype};
    return var;
}

bool VirtualEnvironment::variable_exists(std::string &name) {
    for (auto scope : global_scopes) 
        if (scope.contains(name)) return true;

    if (function_scopes.empty()) return false;
    for (auto subscope : function_scopes.top())
        if (subscope.contains(name)) return true;

    return false;
}

void VirtualEnvironment::set_variable(std::string& name, void* value) {
    get_variable(name).value = value;
}

Variable& VirtualEnvironment::get_variable(std::string& name) {
    for (auto scope : global_scopes) 
        if (scope.contains(name))
            return scope[name];

    for (auto scope : *fscopes_top)
        if (scope.contains(name))
            return scope[name];
}

void VirtualEnvironment::create_scope() {
    if (!is_in_function) {
        global_scopes.push_back({});
        return;
    }
    function_scopes.push(FunctionScope{});
    fscopes_top = &function_scopes.top();
    is_current_scope_global = false;
}

void VirtualEnvironment::destroy_scope() {
    if (!is_in_function) {
        global_scopes.pop_back();
        return;
    }
    fscopes_top->~vector<Scope>();
    function_scopes.pop();
    if (function_scopes.empty())
        is_current_scope_global = true;
    else 
        fscopes_top = &function_scopes.top();

}