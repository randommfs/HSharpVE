#pragma once

// STD
#include <memory>

// Local
// for noncopyable mixin
#include <parser/tokenizer/common-token-readers.hpp>

#define HSHARP_ABORT_IF(condition, message) \
    if ((condition)) { [[maybe_unused]] char __debug_message[] = message; std::abort(); }


namespace hsharp {

    class ArenaDeleter;

    class ArenaAllocator 
        : NonCopyable
        , std::enable_shared_from_this<ArenaAllocator> {
    private: struct Private { };
    public:
        static std::shared_ptr<ArenaAllocator> create(std::size_t bytes) { 
            return std::make_shared<ArenaAllocator>(bytes, Private()); 
        }

        ArenaAllocator(std::size_t bytes, Private access) noexcept
        : allocation_(bytes)
        , freed_(0)
        {
            start_ = static_cast<char*>(std::malloc(bytes));
            HSHARP_ABORT_IF(!start_, "malloc() returned nullptr");

            offset_ = start_;
        } 

        template<typename T>
        T* alloc() noexcept {
            HSHARP_ABORT_IF(allocation_ > sizeof(T) + (offset_ - start_), "not enough space for allocation");

            T* allocated = std::construct_at(static_cast<T*>(offset_));
            offset_ += sizeof(T);
            return allocated;
        }

        template<typename T, typename... Args>
        T* construct(Args&&... args) {
            HSHARP_ABORT_IF(allocation_ > sizeof(T) + (offset_ - start_), "not enough space for allocation");

            T* allocated = std::construct_at(
                static_cast<T*>(offset_), 
                std::forward<Args...>(args...)
            );
            offset_ += sizeof(T);
            return allocated;
        }

        friend ArenaDeleter;

        ~ArenaAllocator() {
            std::free(start_);
        }

    private:
        void free(std::size_t size) {
            freed_ += size;

            if (freed_ == offset_ - start_) {
                offset_ = start_;
                freed_ = 0;
            }
        }

    private:
        std::size_t allocation_;
        std::size_t freed_;

        char* offset_;
        char* start_;

    };

    class ArenaDeleter {
    public:

        ArenaDeleter(ArenaAllocator& allocator) : allocator_(allocator) {}

        template<typename T>
        void operator()(T* pointer) const {
            allocator_.free(sizeof(T));
        }

        // overload for array?

    private:
        ArenaAllocator& allocator_;

    };
}