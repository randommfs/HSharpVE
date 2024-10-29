#include "pog/html_report.h"
#include <iostream>
#include <fstream>

#include <VersionInfo.hpp>
#include <Parser/Parser.hpp>
#include <Main/ArgsHandler.hpp>
#include <Compiler/Compiler_NoOptimizations.hpp>

#include <pog/pog.h>

int main(int argc, char *argv[]) {
    // handle CLI args
    hsharp::ArgumentsHandler argsHandler;
    argsHandler.parse(std::make_unique<hsharp::ArgumentsHandler::SIOArgumentsAccessor>(), argc, argv);
    const hsharp::CLIRegistry& registry = argsHandler.registry();

    for (const hsharp::EDirective& directive : registry.getDirectives()) {
        switch (directive) {
            case hsharp::EDirective::HELP:
                hsharp::displayHelp(std::cout);
                return 0;
            case hsharp::EDirective::VERSION:
                hsharp::displayVersion(std::cout);
                return 0;
        }
    }

    std::string filename = registry.getFilename();

    std::ifstream input(filename);
    std::stringstream contents;
    contents << input.rdbuf();
    if (!input.is_open()) {
        std::cerr << "Cannot open file! Exiting now..." << std::endl;
        exit(1);
    }
    HSharpCompiler::Compiler_NoOpt compiler;
    auto t = compiler.get__parse_literal();
    std::vector<HSharpParser::Token> args = {
        {HSharpParser::TokenType::ASSIGN_ADD, ""}
    };
    t(std::move(args));
    std::cout << std::get<HSharpParser::Token>(compiler.get__parse_literal()(std::move).str);
    HSharpParser::Parser parser{&compiler};
    auto report = parser.prepare();
    std::cout << report.to_string() << '\n';
    parser.parse(contents.str());

    //HSharpVE::VirtualEnvironment ve(root.value(), lines, true); // for now
    //ve.run();
    // Exit point
    input.close();
}