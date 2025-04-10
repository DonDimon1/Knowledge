//#ifndef SHARED_PTR_H_INCLUDED
//#define SHARED_PTR_H_INCLUDED

#pragma once
#include "ControlBlockRegular.h"
#include "ControlBlockMakeShared.h"
#include "enable_shared_from_this.h"


namespace smart {

template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr {

    template <typename U, typename Alloc, typename... Args>
    friend shared_ptr<U> make_shared(Args&&... args);
    /*
	template <typename U, typename... Args>
	friend shared_ptr<U> make_shared(Args&&... args);

	template <typename U, typename Alloc, typename... Args>
	friend shared_ptr<U> allocate_shared_my(const Alloc&, Args&&... args);
    */
	template <typename U>
	friend class weak_ptr;

	T* ptr;
	detail::BaseControlBlock* cb;

	template <typename Alloc>
	shared_ptr(detail::ControlBlockMakeShared<T, Alloc>* cb) : ptr(cb->get_ptr()), cb(cb) {} //TODO: проверить правильно ли &cb->value
	//TODO: это работает только при создании, при копировании нужно сначала очищать

	template <typename U>
	shared_ptr(U* ptr, detail::BaseControlBlock* cb): ptr(ptr), cb(cb) {
        ++cb->detail::BaseControlBlock::shared_count;
	}

public:
	shared_ptr() : ptr(nullptr), cb(nullptr) {}

	template < typename Y,
	typename Deleter = std::default_delete<Y>,
	typename Alloc = std::allocator<Y>>
	shared_ptr(Y* ptr, Deleter del = Deleter(), Alloc alloc = Alloc()) : ptr(ptr) {

		using Block = detail::ControlBlockRegular<Y, Deleter, Alloc>;
		using BlockAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Block>;

		BlockAlloc block_alloc = alloc;
		Block* block = nullptr;

		// Тут надо оборачивать в try
		block = std::allocator_traits<BlockAlloc>::allocate(block_alloc, 1);
		std::allocator_traits<BlockAlloc>::construct(block_alloc, block, ptr, del, alloc, 1, 0);

		cb = block;

		if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>)  { // Для получения shared_ptr, который указывает на нас же (Если нужно).
			ptr->self = *this;  // Тут я не уверен на счёт ptr->sptr
			//block->get()->self = shared_ptr<T>(block->get(), block);
		}
	}

	~shared_ptr() {
		reset();
	}

	shared_ptr(const shared_ptr& other) : ptr(other.ptr), cb(other.cb) {
		++cb->detail::BaseControlBlock::shared_count;
	}

	shared_ptr& operator=(const shared_ptr& other) {
		if (this != &other) {
			reset();

			cb = other.cb;
			ptr = other.ptr;
			++cb->detail::BaseControlBlock::shared_count;
		}
		return *this;
	}

	shared_ptr(shared_ptr&& other) noexcept:
            ptr(other.ptr), cb(other.cb) {
        other.ptr = nullptr;
        other.cb = nullptr;
    }

    shared_ptr& operator=(shared_ptr&& other) noexcept {
        if(this != &other) {
            reset();
            ptr = other.ptr;
            cb = other.cb;
            other.ptr = nullptr;
            other.cb = nullptr;
        }
        return *this;
    }

    void reset() {
		if (cb) {
			--cb->shared_count;
			if (ptr && cb->shared_count == 0) {
				cb->destroy_object();
			}
			if (cb->shared_count == 0 && cb->weak_count == 0) {
				cb->destroy_self();
			}
			ptr = nullptr;
			cb = nullptr;
		}
	}

	size_t use_count() const noexcept {
		return cb ? cb->UseCount() : 0;
	}

	T& operator*() const { return *ptr; }
	T* operator->() const { return ptr; }
	T* get() const { return ptr; }
};

} // namespace smart

//#endif // SHARED_PTR_H_INCLUDED
