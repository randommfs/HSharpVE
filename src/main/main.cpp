#include <iostream>
#include <fstream>
#include <memory>

#include <version.hpp>
#include <parser/parser.hpp>
#include <main/string_split.h>
#include <main/arguments_handler.hpp>

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

    std::ifstream input(filename, std::ios::binary | std::ios::ate);
    if (!input.is_open()) {
        std::cerr << "Cannot open file! Exiting now..." << std::endl;
        exit(1);
    }
    
    HSharpParser::Parser parser{};
    auto report = parser.prepare();

    //HSharpVE::VirtualEnvironment ve(root.value(), lines, true); // for now
    //ve.run();
    // Exit point
    input.close();
}