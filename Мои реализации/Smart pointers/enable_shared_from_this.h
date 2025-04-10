//#ifndef ENABLE_SHARED_FROM_THIS_H_INCLUDED
//#define ENABLE_SHARED_FROM_THIS_H_INCLUDED

#pragma once

namespace smart {

template <typename T>
class shared_ptr;

template <typename T>
class weak_ptr;


template <typename T>
class enable_shared_from_this {
    template <typename U, typename Alloc, typename... Args>
    friend shared_ptr<U> make_shared(Args&&... args);

    mutable weak_ptr<T> self;

public:
    shared_ptr<T> shared_from_this() const {
        return this->self.lock();
    }

    weak_ptr<T> weak_from_this() const noexcept {
        return self;
    }

    template <typename U>
    friend class shared_ptr;
};

} // namespace smart

//#endif // ENABLE_SHARED_FROM_THIS_H_INCLUDED
