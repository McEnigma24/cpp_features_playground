#include "__preprocessor__.h"

#define LIKELY(x)   __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

// Przykład z __builtin_expect (dostępny w GCC i Clang)
void test_builtin_expect(int x)
{
    if (__builtin_expect(x == 0, 1)) { std::cout << "[__builtin_expect] x == 0 (likely path)\n"; }
    else { std::cout << "[__builtin_expect] x != 0 (unlikely path)\n"; }
}

//                                    cond  val prob
// __builtin_expect_with_probability(x == 0, 1, 0.9)

void test_builtin_expect_prob(int x)
{
    if (__builtin_expect_with_probability(x == 0, 1, 0.9)) { std::cout << "[__builtin_expect_with_probability] x == 0 (likely path)\n"; }
    else { std::cout << "[__builtin_expect_with_probability] x != 0 (unlikely path)\n"; }
}

// Przykład z [[likely]] i [[unlikely]] (C++20)
void test_likely_unlikely(int x)
{
    if (x == 0) [[likely]] { std::cout << "[[likely]] x == 0 (likely path)\n"; }
    else [[unlikely]] { std::cout << "[[unlikely]] x != 0 (unlikely path)\n"; }
}

#if __cplusplus >= 202002L
    #define if_likely(cond)   if (cond) [[likely]]
    #define if_unlikely(cond) if (cond) [[unlikely]]
#elif defined(__GNUC__) || defined(__clang__)
    #define if_likely(cond)   if (__builtin_expect(!!(cond), 1))
    #define if_unlikely(cond) if (__builtin_expect(!!(cond), 0))
#else
    #define if_likely(cond)   if (cond)
    #define if_unlikely(cond) if (cond)
#endif

void testing_macros(int x)
{
    if_likely(x == 0) { std::cout << "if_likely \n"; }
    if_unlikely(x == 1) { std::cout << "if_unlikely \n"; }
}

void likely_unlikelying()
{
#define USING_GCC (defined(__GNUC__) && !defined(__clang__))

#if defined(__clang__)
    {
        line("Clang");
    }
#elif USING_GCC
    {
        line("GCC");
    }
#elif defined(_MSC_VER)
    {
        line("MSVC");
    }
#endif

#if __cplusplus >= 202302L
    line("C++23");
#elif __cplusplus >= 202002L
    line("C++20");
#elif __cplusplus >= 201703L
    line("C++17");
#elif __cplusplus >= 201402L
    line("C++14");
#elif __cplusplus >= 201103L
    line("C++11");
#elif __cplusplus >= 199711L
    line("C++98");
#else
    #error "Nieznany lub bardzo stary standard C++"
#endif

    // #if defined(CHUJ)
    //     line("CHUJ defined");
    // #else
    //     #error "Okropna strata, CHUJ nie jest zdefiniowany"
    // #endif

    int test_values[] = {0, 1};

    for (int x : test_values)
    {
        std::cout << "\nTesting with x = " << x << ":\n";
        test_builtin_expect(x);
        test_likely_unlikely(x);
        test_builtin_expect_prob(x);
        testing_macros(x);
        std::cout << "---------------------------------\n";
    }
}