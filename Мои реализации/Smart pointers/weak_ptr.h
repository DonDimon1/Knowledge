//#ifndef WEAK_PTR_H_INCLUDED
//#define WEAK_PTR_H_INCLUDED

#pragma once
//#include "shared_ptr.h"
#include "make_shared.h"

namespace smart {

template <typename T>
class weak_ptr {
    template <typename U, typename Alloc, typename... Args>
    friend shared_ptr<U> make_shared(Args&&... args);

    T* ptr;
    detail::BaseControlBlock* cb;

public:
    weak_ptr() noexcept: ptr(nullptr), cb(nullptr) {}

    template <typename U>
    weak_ptr(const shared_ptr<U>& sp) noexcept:
            ptr(sp.ptr), cb(sp.cb) {
        if(cb)
            ++cb->weak_count;
    }

    weak_ptr(const weak_ptr& other) noexcept:
            ptr(other.ptr), cb(other.cb) {
        if(cb)
            ++cb->weak_count;
    }

    weak_ptr(weak_ptr&& other) noexcept:
            ptr(other.ptr), cb(other.cb) {
        other.ptr = nullptr;
        other.cb = nullptr;
    }

    weak_ptr& operator=(const weak_ptr& other) noexcept {
        if(this != &other) {
            reset();
            ptr = other.ptr;
            cb = other.cb;
            if(cb)
                ++cb->weak_count;
        }
        return *this;
    }

    weak_ptr& operator=(weak_ptr&& other) noexcept {
        if(this != &other) {
            reset();
            ptr = other.ptr;
            cb = other.cb;
            other.ptr = nullptr;
            other.cb = nullptr;
        }
        return *this;
    }

    ~weak_ptr() {
        reset();
    }

    void reset() {
        if (cb) {
			--cb->weak_count;

			if (cb->shared_count == 0 && cb->weak_count == 0) {
				cb->destroy_self();
			}
			ptr = nullptr;
			cb = nullptr;
		}
    }

    shared_ptr<T> lock() const noexcept {
        if(cb && cb->shared_count > 0) {
            return shared_ptr<T>(ptr, cb);
        }
        return shared_ptr<T>();
    }

    bool expired() const noexcept {
        return !cb || cb->shared_count == 0;
    }

    size_t use_count() const noexcept {
		return cb ? cb->UseCount() : 0;
	}
};

} // namespace smart

//#endif // WEAK_PTR_H_INCLUDED
