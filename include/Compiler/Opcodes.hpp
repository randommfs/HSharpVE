#pragma once

#include <cstdint>

#include <mapbox/eternal.hpp>

#define ENTRY(arg) {Opcode::arg, #arg}

namespace HSharpCompiler {
    enum Opcode : std::uint_fast8_t {
        LOAD_CONST,
        PUSH_CONST,
        ADD_BINARY,
        SUB_BINARY,
        MUL_BINARY,
        DIV_BINARY
    };

    constexpr static auto OPCODE_NAMES = mapbox::eternal::map<Opcode, mapbox::eternal::string>({
        ENTRY(LOAD_CONST),
        ENTRY(PUSH_CONST),
        ENTRY(ADD_BINARY),
        ENTRY(SUB_BINARY),
        ENTRY(MUL_BINARY),
        ENTRY(DIV_BINARY)
    });
}