#include "ve/exceptions.hpp"
#include <stdexcept>
#include <type_traits>
#include <visitors.hpp>
#include <ve/ve.hpp>

#include <optional>

#include <cstring>

using HSharpVE::Variable;
using HSharp::VariableType;
using HSharpVE::VirtualEnvironment;

Variable& VirtualEnvironment::create_variable(std::string &name, VariableType vtype) {
    if (is_current_scope_global){
        Variable& ref = global_scopes.back()[name];
        ref = {.vtype = vtype};
        return ref;
    }
    
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

void VirtualEnvironment::set_variable(std::string& name, VariableType type, void* value) {
    if (!variable_exists(name)) {
        error(EExceptionSource::VIRTUAL_ENV,
        EExceptionReason::UNKNOWN_IDENT,
        "Unknown identifier");
    }
    Variable& var = get_variable(name);
    delete_var_value(var);
    var.vtype = type;
    var.value = value;
    int a = 5;
}

void VirtualEnvironment::set_variable(std::string& name, void* value) {
    if (!variable_exists(name)) {
        error(EExceptionSource::VIRTUAL_ENV,
        EExceptionReason::UNKNOWN_IDENT,
        "Unknown identifier");
    }

    Variable& var = get_variable(name);
    delete_var_value(var);
    var.value = value;
}

Variable& VirtualEnvironment::get_variable(std::string& name) {
    std::cout << "Searching in global scope" << std::endl;
    for (auto scope : global_scopes) 
        if (scope.contains(name)) {
            std::printf("ptr: %p\n", &scope.at(name));
            return scope.at(name);
        }

    std::cout << "Searching in function scopes" << std::endl;
    for (auto scope : *fscopes_top)
        if (scope.contains(name))
            return scope.at(name);
    
    error(EExceptionSource::VIRTUAL_ENV,
            EExceptionReason::UNKNOWN_IDENT,
            "Unknown identifier");
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