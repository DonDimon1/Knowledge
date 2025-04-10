//#ifndef CONTROLBLOCKREGULAR_H_INCLUDED
//#define CONTROLBLOCKREGULAR_H_INCLUDED

#pragma once
#include "BaseControlBlock.h"

namespace smart::detail {

template < typename U, typename Deleter, typename Alloc>
    struct ControlBlockRegular : public BaseControlBlock {
    U* ptr;
    Deleter del;
    Alloc alloc;

    ControlBlockRegular(U* ptr, const Deleter& del, const Alloc& alloc, size_t shared_count, size_t weak_count) :
        BaseControlBlock(shared_count, weak_count), ptr(ptr), del(del), alloc(alloc) {
    }

    ~ControlBlockRegular() override = default;

    void destroy_object() override {
        del(ptr);					// При shared_count == 0 удаляем объект.
        ptr = nullptr;				// TODO Нужна ли проверка на nullptr перед del?
    }

    void destroy_self() override {
        using BlockAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<ControlBlockRegular>;
        BlockAlloc block_alloc = alloc;

        std::allocator_traits<BlockAlloc>::destroy(block_alloc, this);
        std::allocator_traits<BlockAlloc>::deallocate(block_alloc, this, 1);
    }
};

}

//#endif // CONTROLBLOCKREGULAR_H_INCLUDED
