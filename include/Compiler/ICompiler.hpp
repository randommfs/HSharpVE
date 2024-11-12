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
        FLOAT,
        BOOL
    };

    using ConstantValue = std::variant<std::string_view, std::int64_t, double, bool>;

    struct Constant {
        ConstantType type;
        ConstantValue value;
    };

    struct CompilerState {
        std::uint32_t consts_cnt;
        std::uint32_t var_names_cnt;
        std::vector<Instruction> instructions;
        std::vector<Constant> consts;
        std::unordered_map<std::string_view, std::uint64_t> funcs;
        std::unordered_map<std::string_view, std::uint32_t> var_names;

        std::uint32_t push_const(std::string_view& value) {
            consts.push_back({ConstantType::STRING, value});
            return consts_cnt++;
        }

        std::uint32_t push_const(std::int64_t& value) {
            consts.push_back({ConstantType::INT, value});
            return consts_cnt++;
        }

        std::uint32_t push_const(double& value) {
            consts.push_back({ConstantType::FLOAT, value});
            return consts_cnt++;
        }

        std::uint32_t push_const(bool value) {
            consts.push_back({ConstantType::BOOL, value});
            return consts_cnt++;
        }

        std::uint32_t allocate_var_name(std::string_view& name) {
            var_names[name] = var_names_cnt;
            return var_names_cnt++;
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

        virtual ParserCallbackType get__transform_expression() noexcept = 0;
        virtual ParserCallbackType get__compile_var_creation() noexcept = 0;
    };
}