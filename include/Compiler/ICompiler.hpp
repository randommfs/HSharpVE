#pragma once

#include <cstdint>
#include <vector>

#include <Parser/Parser.hpp>
#include <Compiler/Opcodes.hpp>

namespace HSharpCompiler {
    struct Instruction {
        Opcode opc;
        std::uint8_t arg;
    };

    enum ConstantType : std::uint_fast8_t {
        STRING,
        INT,
        FLOAT
    };

    using ConstantValue = std::variant<std::string, std::int64_t, double>;

    struct Constant {
        ConstantType type;
        ConstantValue value;
    };

    struct CompilerState {
        std::uint64_t consts_cnt;
        std::vector<Instruction> instructions;
        std::vector<Constant> consts;
        std::unordered_map<std::string_view, std::uint64_t> funcs;

        std::uint64_t push_const(std::string& value) {
            consts.push_back({ConstantType::STRING, value});
            return consts_cnt++;
        }

        std::uint64_t push_const(std::int64_t& value) {
            consts.push_back({ConstantType::INT, value});
            return consts_cnt++;
        }

        std::uint64_t push_const(double& value) {
            consts.push_back({ConstantType::FLOAT, value});
            return consts_cnt++;
        }
    };

    using ParserCallbackType = std::function<HSharpParser::Value(std::vector<HSharpParser::Value>&&)>;
    class ICompiler {
    public:
        virtual void emit_opcode(Opcode op, std::uint8_t arg = 0) = 0;

        /* Handlers for basic expression parts - operators and tokens*/
        virtual ParserCallbackType get__parse_operator() noexcept = 0;
        virtual ParserCallbackType get__parse_literal() noexcept = 0;
        virtual ParserCallbackType get__parse_ident() noexcept = 0;

        virtual ParserCallbackType get__compile_expression() noexcept = 0;
    };
}