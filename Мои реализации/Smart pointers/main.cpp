#include <iostream>
//#include "weak_ptr.h"
//#include <gtest/gtest.h>
#include "test.h"

using namespace smart;

/*
// ������ ��� ������������
struct Array {
	int* p;
	Array(int i) : p(new int[1000] {i}) {}
	~Array() {
		delete[] p;
	}
};

template <typename T>
struct CustomDeleter {
	void operator()(T* ptr) const {
		std::cout << "CustomDeleter" << '\n';
		delete ptr;
	};
};

template <typename T>
struct CustomAllocator {
	using value_type = T;

	CustomAllocator() = default;
	template <typename U>
	CustomAllocator(const CustomAllocator<U>&) noexcept {}

	[[nodiscard]] T* allocate(std::size_t n) {
		if (n > std::allocator_traits<std::allocator<T>>::max_size(std::allocator<T>{})) {
			throw std::bad_alloc();
		}
		std::cout << "Allocating " << n << " objects of size " << sizeof(T) << "\n";
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}

	void deallocate(T* p, std::size_t n) noexcept {
		std::cout << "Deallocating " << n << " objects of size " << sizeof(T) << "\n";
		::operator delete(p);
	}

	template <typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		std::cout << "Constructing object at " << p << "\n";
		::new (static_cast<void*>(p)) U(std::forward<Args>(args)...);
	}

	template <typename U>
	void destroy(U* p) {
		std::cout << "Destroying object at " << p << "\n";
		p->~U();
	}

	template <typename U>
	struct rebind {
		using other = CustomAllocator<U>;
	};

	template <typename U>
	using rebind_alloc = CustomAllocator<U>;

	template <typename U>
	bool operator==(const CustomAllocator<U>&) const noexcept { return true; }
	template <typename U>
	bool operator!=(const CustomAllocator<U>&) const noexcept { return false; }
};
*/

int main(int argc, char **argv)
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// ������������ ������ ������
	//_CrtSetBreakAlloc(82);
	::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();

    /*
	{ // ����������� �� ���������
		shared_ptr<Array> sp;
	}
	{ // ������� �����������
		Array* pa = new Array(65);
		shared_ptr<Array> sp(pa);
	}
	{ // ����������� + CustomDeleter
		Array* pa = new Array(65);
		CustomDeleter<Array> del;
		shared_ptr<Array> sp(pa, del);
	}
	{ // ����������� + CustomDeleter + CustumAllocator
		Array* pa = new Array(65);
		CustomDeleter<Array> del;
		std::allocator<Array> alloc;
		std::cout << "sizeof(Array* pa) " << sizeof(pa) << '\n'; //TODO: �� ���������� 8 �.�. ���������
		shared_ptr<Array> sp(pa, del, alloc);
	}
	{ // �������� make_shared
		shared_ptr<Array> sp = make_shared<Array>(65);
	}
	{ // �������� ������� ������������� � �������� ������������ shared_ptr
		Array* pa = new Array(65);
		shared_ptr<Array> sp(pa);
		shared_ptr<Array> sp2(sp);
	}
	{ // �������� make_shared � �������� ������������ shared_ptr
		shared_ptr<Array> sp = make_shared<Array>(65);
		shared_ptr<Array> sp2(sp);
	}
	{ // �������� ������� �������������, �������� make_shared, ������������ ������� ��������� �������
		Array* pa = new Array(65);
		shared_ptr<Array> sp(pa);
		shared_ptr<Array> sp2 = make_shared<Array>(56);
		sp = sp2;
	}
	{ // �������� ������� �������������, �������� make_shared, ������������ ������� ��������� �������
		Array* pa = new Array(65);
		shared_ptr<Array> sp(pa);
		shared_ptr<Array> sp2 = make_shared<Array>(56);
		sp2 = sp;
	}
	{ // �������� allocate_shared c std::allocator
		std::allocator<Array> alloc;
		//shared_ptr<Array> sp = allocate_shared_my<Array>(alloc, 65);
	}
	{ // �������� allocate_shared c CustomAllocator
		//CustomAllocator<Array> alloc;
		//shared_ptr<Array> sp = allocate_shared_my<Array>(alloc, 65);
	}
	// TODO:: ���� �������� ����������, ����� ��� ������������ ��� ControlBlockRegular, ����� make_shared, ����� ��� ������������ ��� ControlBlockMakeShared, ����� �������� ����� ��� ���-�� ����.
	// TODO: � ������� ������ �� ����� ���������������� ������������, �� ����� ������ ����������� ������� getCopy() ��� �������� ����� ���� ������� ������, �� ����� ������������ ��� ������� ����� � �.�.
    return 0;*/


}
