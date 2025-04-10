//#ifndef CONTROLBLOCKMAKESHARED_H_INCLUDED
//#define CONTROLBLOCKMAKESHARED_H_INCLUDED

#pragma once
#include "BaseControlBlock.h"

namespace smart::detail {

template < typename U, typename Alloc>
struct ControlBlockMakeShared : public BaseControlBlock {
    //U value;	// Если оставить так, то не получится управлять памятью с помощью аллокатора
    alignas(U) unsigned char storage[sizeof(U)];
    Alloc alloc_;

    U* get_ptr() { return reinterpret_cast<U*>(&storage); }

    template <typename... Args>
    ControlBlockMakeShared(size_t shared_count, size_t weak_count, const Alloc& alloc, Args&&... args):
            BaseControlBlock(shared_count, weak_count), alloc_(alloc) {
        std::allocator_traits<Alloc>::construct(alloc_, get_ptr(), std::forward<Args>(args)...);
    }

    ~ControlBlockMakeShared() = default;

    void destroy_object() override {
        //value.~U();   // Двойное удаление
        std::allocator_traits<Alloc>::destroy(alloc_, get_ptr());
    };
    void destroy_self() override {
        using BlockAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<detail::ControlBlockMakeShared<U, Alloc>>;
        BlockAlloc ba = alloc_;
        std::allocator_traits<BlockAlloc>::destroy(ba, this);
        std::allocator_traits<BlockAlloc>::deallocate(ba, this, 1);
    };
};
}

//#endif // CONTROLBLOCKMAKESHARED_H_INCLUDED
