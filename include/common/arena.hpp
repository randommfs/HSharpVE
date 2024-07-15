#pragma once

// STD
#include <memory>

// Local
// for noncopyable mixin
#include <parser/tokenizer/common-token-readers.hpp>

namespace hsharp {

    class ArenaAllocator 
        : NonCopyable
        , std::enable_shared_from_this<ArenaAllocator> {
    private: struct Private { };
    public:

        static std::shared_ptr<ArenaAllocator> create(std::size_t bytes);
        ArenaAllocator(std::size_t bytes, Private access);

        template<typename T>
        T* alloc() noexcept;

        template<typename T, typename... Args>
        T* construct(Args&&... args);

        ~ArenaAllocator();

    private:
        std::size_t allocation_;
        char* offset_;
        char* start_;

    };
}