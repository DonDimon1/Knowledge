//#ifndef MAKE_SHARED_H_INCLUDED
//#define MAKE_SHARED_H_INCLUDED

#pragma once

#include "shared_ptr.h"
namespace smart {

template <typename T, typename Alloc = std::allocator<T>, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
	//using Block = typename shared_ptr<T>::template detail::ControlBlockMakeShared<T, Alloc>;
	using Block = detail::ControlBlockMakeShared<T, Alloc>;
	using BlockAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Block>;

	BlockAlloc alloc;
	Block* block = std::allocator_traits<BlockAlloc>::allocate(alloc, 1);
	std::allocator_traits<BlockAlloc>::construct(alloc, block, 1, 0, alloc, std::forward<Args>(args)...);

    shared_ptr<T> sp(block);

    if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>)  { // Для получения shared_ptr, который указывает на нас же (Если нужно).
        sp.ptr->self = sp;
    }

    return sp;
	//return shared_ptr<T>(block);
}

} // namespace smart

//#endif // MAKE_SHARED_H_INCLUDED
