#include "__preprocessor__.h"

std::vector<uint64_t> factorization(uint64_t n)
{
    if (n < 2) return {};

    uint64_t stop = std::sqrt(n);
    std::vector<uint64_t> decomposed;

    for (uint64_t i = 2; i <= stop; i++)
    {
        while ((n % i) == 0)
        {
            decomposed.push_back(i);
            n /= i;
        }
        // nie możemy tak skakać, bo tak to pomijamy inne liczby pierwsze //

        // jedyne co można robić to dodawać na listę liczb pierwszych i sprawdzać czy następna liczba jest przez nią podzielna //
        // jak jest to idziemy dalej //

        // to albo zrobić tablicę gdzie index + 1 to liczba --> i skaczemy po niej o właśnie tyle ile teraz wyczerpaliśmy //
        // zaznaczając na true, że tą liczbę już sprawdzaliśmy //
    }

    return decomposed;
}

int main()
{
    for (int i = 0; i < 100; i++)
    {
        auto factors = factorization(i);
        std::cout << i << ": ";
        for (int x = 0; x < factors.size(); x++)
        {
            auto factor = factors[x];
            std::cout << factor;
            if (x != factors.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}