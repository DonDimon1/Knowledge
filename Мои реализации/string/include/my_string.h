#ifndef MY_STRING_H
#define MY_STRING_H

//-std=c++20
#include <gtest/gtest.h>

#include <iostream>
#include <cstring>
#include <algorithm>


//class my_string_test;

class my_string {
    //friend class my_string_test;
private:
    FRIEND_TEST(Ctor, DefaultCtor);
    FRIEND_TEST(Ctor, CtorAllocStack);
    FRIEND_TEST(Ctor, CtorAllocHeap);
    FRIEND_TEST(Ctor, CtorStrAllocStack);
    FRIEND_TEST(Ctor, CtorStrAllocHeap);
    FRIEND_TEST(operator_equal, StackToStack);
    FRIEND_TEST(operator_equal, StackToHeap);
    FRIEND_TEST(operator_equal, HeapToStack);
    FRIEND_TEST(operator_equal, HeapToHeapEnoughMemory);
    FRIEND_TEST(operator_equal, HeapToHeapNotEnoughMemory);
    FRIEND_TEST(operator_equal_str, StackToStack);
    FRIEND_TEST(operator_equal_str, StackToHeap);
    FRIEND_TEST(operator_equal_str, HeapToStack);
    FRIEND_TEST(operator_equal_str, HeapToHeapEnoughMemory);
    FRIEND_TEST(operator_equal_str, HeapToHeapNotEnoughMemory);
    FRIEND_TEST(operator_equal_rvalue, StackToStack);
    FRIEND_TEST(operator_equal_rvalue, StackToHeap);
    FRIEND_TEST(operator_equal_rvalue, HeapToStack);
    FRIEND_TEST(operator_equal_rvalue, HeapToHeapEnoughMemory);
    FRIEND_TEST(operator_equal_rvalue, HeapToHeapNotEnoughMemory);

private:
    static constexpr size_t _MAX_STACK_SIZE = 15 / sizeof(char);

    //char* _buffer = nullptr;
    size_t _size;
    bool _is_heap;
    char* _data;

    union {
        size_t _capacity;
        char _stack[_MAX_STACK_SIZE + 1];
    };

private:
    /*
    void AllocateMemory(size_t newSize) {
        _size = newSize;
        if(!_is_heap) {                 // ���� ��� �� �����
            _heap.ptr = new char[_size + 1];
            _heap.capacity = _size;     // TODO: ������ � capacity
            _is_heap = true;
        }
        else if(_heap.capacity < _size) {    // ���� ��� �� ���� � ����� �� �������
            delete _heap.ptr;
            _heap.ptr = new char[_size + 1];
            _heap.capacity = _size;     // TODO: ������ � capacity
        }
    }*/

public:
    my_string() noexcept: _size(0), _is_heap(false), _data(&_stack[0]) {
        _stack[0] = '\0';
    }

    my_string(const char* newData) {
        if(newData == nullptr) {
            _size = 0;
            _is_heap = false;
            _data = &_stack[0];
            *_data = '\0';
            return;
        }

        _size = std::strlen(newData);
        if(_size < _MAX_STACK_SIZE) {
            _is_heap = false;
            _data = &_stack[0];
            std::strcpy(_data, newData);
            _data[_size] = '\0';
        } else {
            _is_heap = true;
            _capacity = _size;                         // TODO: ������ � capacity
            _data = new char[_size + 1];
            std::strcpy(_data, newData);
            _data[_size] = '\0';
        }
    }
    my_string(const my_string& other) {
        _size = other._size;
        if(_size < _MAX_STACK_SIZE) {
            _is_heap = false;
            _data = &_stack[0];
            std::strcpy(_data, other._data);
        } else {
            _is_heap = true;
            _capacity = _size;                         // TODO: ������ � capacity
            _data = new char[_size + 1];
            std::strcpy(_data, other._data);
            _data[_size] = '\0';
        }
    }
    my_string& operator=(const char* newData) {       // TODO: ������� ������� � ������� ������ ��� � �������
        if(newData != nullptr) {
            _size = std::strlen(newData);

            if(_size < _MAX_STACK_SIZE) {       // ���� �� �����
                if(_is_heap) {                  // ���� ��� �� ����
                    delete _data;
                    _data = &_stack[0];
                    _is_heap = false;
                }

                std::strcpy(_data, newData);
                _data[_size] = '\0';
            }
            else {                              // ���� �� ����
                if(!_is_heap) {                 // ���� ��� �� �����
                    _data = new char[_size + 1];
                    _capacity = _size;     // TODO: ������ � capacity
                    _is_heap = true;
                }

                else if(_capacity < _size) {    // ���� ��� �� ���� � ����� �� �������
                    delete _data;
                    _data = new char[_size + 1];
                    _capacity = _size;     // TODO: ������ � capacity
                }

                std::strcpy(_data, newData);
                _data[_size] = '\0';
            }
        }
        return *this;
    }
    my_string& operator=(const my_string& other) {
        if(this != &other) {
            _size = other._size;

            if(_size < _MAX_STACK_SIZE) {       // ���� �� �����
                if(_is_heap) {                  // ���� ��� �� ����
                    delete _data;
                    _data = &_stack[0];
                    _is_heap = false;
                }

                std::strcpy(_data, other._data);
                _data[_size] = '\0';
            }
            else {                              // ���� �� ����
                if(!_is_heap) {                 // ���� ��� �� �����
                    _data = new char[_size + 1];
                    _capacity = _size;     // TODO: ������ � capacity
                    _is_heap = true;
                }
                else if(_capacity < _size) {    // ���� ��� �� ���� � ����� �� �������
                    delete _data;
                    _data = new char[_size + 1];
                    _capacity = _size;     // TODO: ������ � capacity
                }

                std::strcpy(_data, other._data);
                _data[_size] = '\0';
            }
        }
        return *this;
    }
    my_string& operator=(my_string&& other) {
        if(this != &other) {
            _size = other._size;

            if(_size < _MAX_STACK_SIZE) {       // ���� �� �����
                if(_is_heap) {                  // ���� ��� �� ����
                    delete _data;
                    _data = &_stack[0];
                    _is_heap = false;
                }
                // �������� ������ � ��������� ����� � ����� ������, ��������� ������ �� ����� ��� �� ����
                std::strcpy(_data, other._data);
                _data[_size] = '\0';

                other._size = 0;
                other._data = &other._stack[0];
                other._data[0] = '\0';
                other._is_heap = false;
            }
            else {                              // ���� �� ����
                if(_is_heap)
                    delete _data;

                _is_heap = true;
                _data = other._data;
                _capacity = other._capacity;

                other._size = 0;
                other._data = &other._stack[0];
                other._data[0] = '\0';
                other._is_heap = false;
            }
        }
        return *this;
    }
    ~my_string() {
        _size = 0;
        if(_is_heap) {
            _capacity = 0;
            delete _data;
        }
    }

    bool operator<(const my_string& other) const {
        //if(!_is_heap || !other._is_heap) {                    // ���� ���� �� �����
            size_t min_size = std::min(_size, other._size);

            for(size_t i = 0; i < min_size; ++i) {
                if(_data[i] < other._data[i])
                    return true;
                else if(_data[i] > other._data[i])
                    return false;
            }
            return _size < other._size ? true : false;
        //}
        //else                                                // ���� �� � ����
            //return _data < other._data;                       // TODO:: ������-�� �� ������ �������� ���������, ������ ��-�� ���������� �������
    }
    bool operator>(const my_string& other) const {
        return other < *this;
    }
    bool operator<=(const my_string& other) const {
        return !(other < *this);
    }
    bool operator>=(const my_string& other) const {
        return !(*this < other);
    }
    bool operator==(const my_string& other) const {
        return !(*this < other ) && !(other < *this);
    }
    bool operator!=(const my_string& other) const {
        return !(*this == other);
    }
    char operator[](size_t num) const {
        return _data[num];
    }
    my_string operator+(const my_string& other) {
        my_string temp;
        temp._size = _size + other._size;

        if(temp._size > _MAX_STACK_SIZE) {
            temp._data = new char[temp._size + 1];          // ������ size ���� capacity
            temp._data[temp._size] = '\0';                  // ������ size ���� capacity
            temp._data[0] = '\0';
            temp._capacity = temp._size;                    // TODO:: ������ � capacity
            temp._is_heap = true;
        }

        std::strcat(temp._data, _data);
        std::strcat(temp._data, other._data);

        return temp;
    }

    friend std::ostream& operator<<(std::ostream& os, const my_string& str) {
		return os << str._data;
	}

    size_t size() const {
        return _size;
    }

    /*
    friend std::ostream& operator<<(std::ostream& os, const string& str) {
		return os << str._buffer;
	}

	void print() {
		if(_size > 0)
			std::cout << _buffer << std::endl;
		else
			std::cout << "" << std::endl;
	}*/
};


#endif // MY_STRING_H
