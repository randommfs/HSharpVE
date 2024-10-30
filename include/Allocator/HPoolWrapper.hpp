#pragma once

#include <math.h>

#include <hpool.hpp>

namespace HSharpAllocator {
    namespace _internal {
        template<typename... T>
        constexpr std::size_t _block_size = std::max({sizeof(T)...});
    }

    template<typename... T_>
    class Allocator {
    private:
        using _alloc_type = char[_internal::_block_size<T_...>];
        using HPool = hpool::HPool<_alloc_type, hpool::ReallocationPolicy::OffsetRealloc>;

        HPool pool_;
    public:
        Allocator() : pool_(16) { }

        template<typename T>
        T* allocate() noexcept {
            return reinterpret_cast<T*>(pool_.allocate());
        }

        template<typename T>
        void free(T* ptr) {
            pool_.free(reinterpret_cast<_alloc_type*>(ptr));
        }
    };
}