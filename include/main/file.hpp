#pragma once

#include <string>
#include <optional>

#include <cinttypes>

struct File{
    std::optional<std::string> contents;
    std::uint64_t size;
    File(){}
    File(std::string str) : contents(str){}
};