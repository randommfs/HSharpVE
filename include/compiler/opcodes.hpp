#pragma once

#include <cstdint>

#include <mapbox/eternal.hpp>

#define STRINGIFY(arg) #arg

namespace HSharpCompiler {
    enum Opcode : std::uint_fast8_t {
        PUSH_CONST,
        ADD_BINARY
    };

    constexpr static auto OPCODE_NAMES = mapbox::eternal::map<Opcode, mapbox::eternal::string>({
        {Opcode::PUSH_CONST, STRINGIFY(PUSH_CONST)},
        {Opcode::ADD_BINARY, STRINGIFY(ADD_BINARY)}
    });
}