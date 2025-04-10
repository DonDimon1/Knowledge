//#ifndef TEST_H_INCLUDED
//#define TEST_H_INCLUDED

#include <gtest/gtest.h>
#include "weak_ptr.h"


using namespace smart;

const int size_arr = 1000;

// Классы для тестирования
struct Array {
	int* p;
	static int instance_count;

	Array(int j) : p(new int[size_arr]) {
        ++instance_count;

        for(int i = 0; i < size_arr; ++i) {
            *(p + i) = i * j;
        }
	}
	~Array() {
		delete[] p;
		--instance_count;
	}
};

int Array::instance_count = 0;


template <typename T>
struct CustomDeleter {
	void operator()(T* ptr) const {
		//std::cout << "CustomDeleter" << '\n';
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
		//std::cout << "Allocating " << n << " objects of size " << sizeof(T) << "\n";
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}

	void deallocate(T* p, std::size_t n) noexcept {
		//std::cout << "Deallocating " << n << " objects of size " << sizeof(T) << "\n";
		::operator delete(p);
	}

	template <typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		//std::cout << "Constructing object at " << p << "\n";
		::new (static_cast<void*>(p)) U(std::forward<Args>(args)...);
	}

	template <typename U>
	void destroy(U* p) {
		//std::cout << "Destroying object at " << p << "\n";
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


// shared_ptr
TEST(SharedPtrTest, DefaultCtor) { // Конструктор по умолчанию
    {
		shared_ptr<Array> sp;
		EXPECT_EQ(sp.get(), nullptr);
		EXPECT_EQ(sp.use_count(), size_t(0));
		EXPECT_EQ(Array::instance_count, 0);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(SharedPtrTest, CtorOneArgument) { // Обычный конструктор
    {
            Array* pa = new Array(65);
            shared_ptr<Array> sp(pa);

            EXPECT_NE(sp.get(), nullptr);
            EXPECT_EQ(*sp.get()->p, 0);
            EXPECT_EQ(*(sp.get()->p + 999), 64935);
            EXPECT_EQ(Array::instance_count, 1);
            EXPECT_EQ(sp.use_count(), 1);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(SharedPtrTest, CtorCustomDeleter) { // Конструктор + CustomDeleter
    {
        Array* pa = new Array(65);
		CustomDeleter<Array> del;
		shared_ptr<Array> sp(pa, del);

		EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 1);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(SharedPtrTest, CtorSTD_Allocator) { // Конструктор + CustomDeleter + std::Allocator
    {
		Array* pa = new Array(65);
		CustomDeleter<Array> del;
		std::allocator<Array> alloc;
		shared_ptr<Array> sp(pa, del, alloc);

		EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 1);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(SharedPtrTest, CtorCustomAllocator) { // Конструктор + CustomDeleter + CustomAllocator
    {
		Array* pa = new Array(65);
		CustomDeleter<Array> del;
		//std::allocator<Array> alloc;
		CustomAllocator<Array> alloc;
		shared_ptr<Array> sp(pa, del, alloc);

		EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 1);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(SharedPtrTest, CopyCtor) { // Копи конструктор
    {
        Array* pa = new Array(65);
        shared_ptr<Array> sp(pa);

        EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(*sp.get()->p, 0);
        EXPECT_EQ(*(sp.get()->p + 999), 64935);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 1);

        shared_ptr<Array> sp2(sp);

        EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(*sp.get()->p, 0);
        EXPECT_EQ(*(sp.get()->p + 999), 64935);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 2);

        EXPECT_NE(sp2.get(), nullptr);
        EXPECT_EQ(*sp2.get()->p, 0);
        EXPECT_EQ(*(sp2.get()->p + 999), 64935);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 2);

        EXPECT_EQ(sp2.get, sp2.get);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(SharedPtrTest, MoveCtor) { // Мув конструктор
    {
        Array* pa = new Array(65);
        shared_ptr<Array> sp(pa);

        EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(*sp.get()->p, 0);
        EXPECT_EQ(*(sp.get()->p + 999), 64935);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 1);

        shared_ptr<Array> sp2(std::move(sp));

        EXPECT_EQ(sp.get(), nullptr);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 0);

        EXPECT_NE(sp2.get(), nullptr);
        EXPECT_EQ(*sp2.get()->p, 0);
        EXPECT_EQ(*(sp2.get()->p + 999), 64935);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp2.use_count(), 1);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(SharedPtrTest, CopyOperator) { // Копи оператор
    {
        Array* pa = new Array(65);
        shared_ptr<Array> sp(pa);

        EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(*sp.get()->p, 0);
        EXPECT_EQ(*(sp.get()->p + 999), 64935);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 1);

        Array* pa2 = new Array(56);
        shared_ptr<Array> sp2(pa2);

        EXPECT_NE(sp2.get(), nullptr);
        EXPECT_EQ(*sp2.get()->p, 0);
        EXPECT_EQ(*(sp2.get()->p + 999), 55944);
        EXPECT_EQ(Array::instance_count, 2);
        EXPECT_EQ(sp2.use_count(), 1);

        sp = sp2;

        EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(*sp.get()->p, 0);
        EXPECT_EQ(*(sp.get()->p + 999), 55944);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 2);

        EXPECT_NE(sp2.get(), nullptr);
        EXPECT_EQ(*sp2.get()->p, 0);
        EXPECT_EQ(*(sp2.get()->p + 999), 55944);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp2.use_count(), 2);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(SharedPtrTest, MoveOperator) { // Мув оператор
    {
        Array* pa = new Array(65);
        shared_ptr<Array> sp(pa);

        EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(*sp.get()->p, 0);
        EXPECT_EQ(*(sp.get()->p + 999), 64935);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 1);

        Array* pa2 = new Array(56);
        shared_ptr<Array> sp2(pa2);

        EXPECT_NE(sp2.get(), nullptr);
        EXPECT_EQ(*sp2.get()->p, 0);
        EXPECT_EQ(*(sp2.get()->p + 999), 55944);
        EXPECT_EQ(Array::instance_count, 2);
        EXPECT_EQ(sp2.use_count(), 1);

        sp = std::move(sp2);

        EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(*sp.get()->p, 0);
        EXPECT_EQ(*(sp.get()->p + 999), 55944);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 1);

        EXPECT_EQ(sp2.get(), nullptr);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp2.use_count(), 0);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(SharedPtrTest, MakeShared) { // MakeShared
    {
        shared_ptr<Array> sp = make_shared<Array>(65);

        EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(*sp.get()->p, 0);
        EXPECT_EQ(*(sp.get()->p + 999), 64935);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 1);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(SharedPtrTest, DefaultCtor_And_MakeShared) {
    {
        Array* pa = new Array(65);
        shared_ptr<Array> sp(pa);

        EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(*sp.get()->p, 0);
        EXPECT_EQ(*(sp.get()->p + 999), 64935);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 1);

        shared_ptr<Array> sp2 = make_shared<Array>(56);

        EXPECT_NE(sp2.get(), nullptr);
        EXPECT_EQ(*sp2.get()->p, 0);
        EXPECT_EQ(*(sp2.get()->p + 999), 55944);
        EXPECT_EQ(Array::instance_count, 2);
        EXPECT_EQ(sp2.use_count(), 1);

        sp = sp2;

        EXPECT_NE(sp.get(), nullptr);
        EXPECT_EQ(*sp.get()->p, 0);
        EXPECT_EQ(*(sp.get()->p + 999), 55944);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp.use_count(), 2);

        EXPECT_NE(sp2.get(), nullptr);
        EXPECT_EQ(*sp2.get()->p, 0);
        EXPECT_EQ(*(sp2.get()->p + 999), 55944);
        EXPECT_EQ(Array::instance_count, 1);
        EXPECT_EQ(sp2.use_count(), 2);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

// weak_ptr
TEST(WeakPtrTest, ConstructFromShared) { // Создаём weak_ptr из shared_ptr
    {
        shared_ptr<Array> sp(new Array(10));
        weak_ptr<Array> wp(sp);

        EXPECT_EQ(sp.use_count(), 1);
        EXPECT_FALSE(wp.expired());
        EXPECT_EQ(Array::instance_count, 1);

        shared_ptr<Array> locked = wp.lock();

        //ASSERT_TRUE(locked);  //TODO
        EXPECT_EQ(locked.get(), sp.get());
        EXPECT_EQ(*(locked->p + 999), 9990);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(WeakPtrTest, ExpiredAfterSharedDestroyed) {
    weak_ptr<Array> wp;
    {
        shared_ptr<Array> sp(new Array(7));
        wp = sp;

        EXPECT_FALSE(wp.expired());
        EXPECT_EQ(sp.use_count(), 1);
        EXPECT_EQ(Array::instance_count, 1);
    }
    EXPECT_TRUE(wp.expired());

    auto locked = wp.lock();
    EXPECT_EQ(locked.get(), nullptr);
    EXPECT_EQ(Array::instance_count, 0);
}

TEST(WeakPtrTest, CopyConstructorAndAssignment) {
    {
        shared_ptr<Array> sp(new Array(3));
        weak_ptr<Array> wp1(sp);
        weak_ptr<Array> wp2 = wp1;

        EXPECT_FALSE(wp2.expired());

        auto locked = wp2.lock();
        //EXPECT_TRUE(locked);  // TODO
        EXPECT_EQ(locked->p[999], 2997);
        EXPECT_EQ(sp.use_count(), 2);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(WeakPtrTest, ResetBehavior) {
    {
        shared_ptr<Array> sp(new Array(5));
        weak_ptr<Array> wp(sp);

        EXPECT_FALSE(wp.expired());
        wp.reset();
        EXPECT_TRUE(wp.expired());
        EXPECT_EQ(wp.lock().get(), nullptr);
        EXPECT_EQ(sp.use_count(), 1);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

TEST(WeakPtrTest, UseCountAndExpired) {
    {
        shared_ptr<Array> sp(new Array(2));
        weak_ptr<Array> wp(sp);

        EXPECT_EQ(wp.use_count(), 1);
        EXPECT_FALSE(wp.expired());

        sp.reset();  // Уничтожаем shared_ptr

        EXPECT_EQ(wp.use_count(), 0);
        EXPECT_TRUE(wp.expired());
    }
    ASSERT_EQ(Array::instance_count, 0);
}


struct MyClass : public smart::enable_shared_from_this<MyClass> {
    int value;
    MyClass(int v) : value(v) {}
};

// Проверка shared_from_this внутри объекта
TEST(EnableSharedFromThisTest, SharedFromThisWorks) {
    {
        shared_ptr<MyClass> sp1 = smart::make_shared<MyClass>(42);
        shared_ptr<MyClass> sp2 = sp1->shared_from_this();

        EXPECT_EQ(sp1.get(), sp2.get());                // оба указывают на один объект
        EXPECT_EQ(sp1.use_count(), 2);                  // use_count увеличился
        EXPECT_EQ(sp2->value, 42);                      // объект цел
    }
    ASSERT_EQ(Array::instance_count, 0);
}

// Проверка weak_from_this()
TEST(EnableSharedFromThisTest, WeakFromThisWorks) {
    {
        shared_ptr<MyClass> sp = smart::make_shared<MyClass>(100);
        weak_ptr<MyClass> wp = sp->weak_from_this();

        EXPECT_FALSE(wp.expired());                     // объект жив
        shared_ptr<MyClass> sp2 = wp.lock();

        EXPECT_EQ(sp.get(), sp2.get());
        EXPECT_EQ(sp.use_count(), 2);
    }
    ASSERT_EQ(Array::instance_count, 0);
}

// Проверка после reset()
TEST(EnableSharedFromThisTest, ResetClearsWeak) {
    {
        weak_ptr<MyClass> wp;
        {
            shared_ptr<MyClass> sp = smart::make_shared<MyClass>(999);
            wp = sp->weak_from_this();
            EXPECT_FALSE(wp.expired());                 // ещё жив
        }
        EXPECT_TRUE(wp.expired());                      // объект уничтожен
    }
    ASSERT_EQ(Array::instance_count, 0);
}

/*
// Проверка shared_from_this без shared_ptr (ожидаем std::bad_weak_ptr)
TEST(EnableSharedFromThisTest, SharedFromThisThrows) {
    MyClass* raw = new MyClass(123);
    EXPECT_THROW({
        raw->shared_from_this();                    // нет связанного shared_ptr
    }, std::bad_weak_ptr);
    delete raw;
}*/
