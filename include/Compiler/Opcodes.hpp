#pragma once

#include <cstdint>

#include <mapbox/eternal.hpp>

#define ENTRY(arg) {Opcode::arg, #arg}

namespace HSharpCompiler {
    enum Opcode : std::uint_fast8_t {
        PUSH_CONST,
        ADD_BINARY
    };

    constexpr static auto OPCODE_NAMES = mapbox::eternal::map<Opcode, mapbox::eternal::string>({
        ENTRY(PUSH_CONST),
        ENTRY(ADD_BINARY)
    });
}