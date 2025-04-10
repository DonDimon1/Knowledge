//#ifndef BASECONTROLBLOCK_H_INCLUDED
//#define BASECONTROLBLOCK_H_INCLUDED
#pragma once

#include <memory>

namespace smart::detail {

struct BaseControlBlock {
    size_t shared_count;	//TODO:: м.б. инициализировать поля надо тут?
    size_t weak_count;

    BaseControlBlock() : shared_count(0), weak_count(0) {}

    BaseControlBlock(size_t shared, size_t weak)
        : shared_count(shared), weak_count(weak) {
    }

    virtual ~BaseControlBlock() = 0;
    virtual void destroy_object() = 0;
    virtual void destroy_self() = 0;

    virtual size_t UseCount() const noexcept {
        return shared_count;
    }
};

BaseControlBlock::~BaseControlBlock() {}

}
//#endif // BASECONTROLBLOCK_H_INCLUDED
