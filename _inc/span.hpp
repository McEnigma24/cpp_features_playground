#include "__preprocessor__.h"
#include <span>

void span_play()
{
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    std::span<int> sp(arr.data(), arr.size() - 2);

    for (auto& elem : sp)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // Modify the array through the span
    for (auto& elem : sp)
    {
        elem = elem * elem;
    }

    for (const auto& elem : arr)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}