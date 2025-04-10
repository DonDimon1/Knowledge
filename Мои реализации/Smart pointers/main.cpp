#include <iostream>
//#include "weak_ptr.h"
//#include <gtest/gtest.h>
#include "test.h"

using namespace smart;

/*
// Классы для тестирования
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
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// Отслеживание утечек памяти
	//_CrtSetBreakAlloc(82);
	::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();

    /*
	{ // Конструктор по умолчанию
		shared_ptr<Array> sp;
	}
	{ // Обычный конструктор
		Array* pa = new Array(65);
		shared_ptr<Array> sp(pa);
	}
	{ // Конструктор + CustomDeleter
		Array* pa = new Array(65);
		CustomDeleter<Array> del;
		shared_ptr<Array> sp(pa, del);
	}
	{ // Конструктор + CustomDeleter + CustumAllocator
		Array* pa = new Array(65);
		CustomDeleter<Array> del;
		std::allocator<Array> alloc;
		std::cout << "sizeof(Array* pa) " << sizeof(pa) << '\n'; //TODO: он показывает 8 т.е. указатель
		shared_ptr<Array> sp(pa, del, alloc);
	}
	{ // Создание make_shared
		shared_ptr<Array> sp = make_shared<Array>(65);
	}
	{ // Создание обычным конструктором и создание копированием shared_ptr
		Array* pa = new Array(65);
		shared_ptr<Array> sp(pa);
		shared_ptr<Array> sp2(sp);
	}
	{ // Создание make_shared и создание копированием shared_ptr
		shared_ptr<Array> sp = make_shared<Array>(65);
		shared_ptr<Array> sp2(sp);
	}
	{ // Создание обычным конструктором, создание make_shared, присваивание первому указателю второго
		Array* pa = new Array(65);
		shared_ptr<Array> sp(pa);
		shared_ptr<Array> sp2 = make_shared<Array>(56);
		sp = sp2;
	}
	{ // Создание обычным конструктором, создание make_shared, присваивание второму указателю первому
		Array* pa = new Array(65);
		shared_ptr<Array> sp(pa);
		shared_ptr<Array> sp2 = make_shared<Array>(56);
		sp2 = sp;
	}
	{ // Создание allocate_shared c std::allocator
		std::allocator<Array> alloc;
		//shared_ptr<Array> sp = allocate_shared_my<Array>(alloc, 65);
	}
	{ // Создание allocate_shared c CustomAllocator
		//CustomAllocator<Array> alloc;
		//shared_ptr<Array> sp = allocate_shared_my<Array>(alloc, 65);
	}
	// TODO:: надо написать деструктор, потом все конструкторы для ControlBlockRegular, потом make_shared, потом все конструкторы для ControlBlockMakeShared, потом посмотри фотки там что-то есть.
	// TODO: В контрол блоках мы пишем соответствуюущие конструкторы, мы также делаем виртуальные функции getCopy() для создания копий этих контрол блоков, мы также поддерживаем мув контрол блока и т.д.
    return 0;*/


}
