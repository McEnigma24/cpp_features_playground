#include "__preprocessor__.h"

// Przykład z __builtin_expect (dostępny w GCC i Clang)
void test_builtin_expect(int x)
{
    if (__builtin_expect(x == 0, 1)) { std::cout << "[__builtin_expect] x == 0 (likely path)\n"; }
    else { std::cout << "[__builtin_expect] x != 0 (unlikely path)\n"; }
}

// Przykład z [[likely]] i [[unlikely]] (C++20)
void test_likely_unlikely(int x)
{
    if (x == 0) [[likely]] { std::cout << "[[likely]] x == 0 (likely path)\n"; }
    else [[unlikely]] { std::cout << "[[unlikely]] x != 0 (unlikely path)\n"; }
}

int main()
{
    int test_values[] = {0, 1};

    for (int x : test_values)
    {
        std::cout << "\nTesting with x = " << x << ":\n";
        test_builtin_expect(x);
        test_likely_unlikely(x);
    }

    return 0;
}

void likely_unlikely() {}