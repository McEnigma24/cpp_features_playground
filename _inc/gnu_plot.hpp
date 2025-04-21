#include "__preprocessor__.h"
#include "gplot++.h"

void gnu_plot()
{
    Gnuplot gnuplot{};
    std::vector<double> x{1, 2, 3, 4, 5}, y{5, 2, 4, 1, 3};

    gnuplot.redirect_to_png("output.png", "800,600");

    gnuplot.plot(x, y);
    gnuplot.show();
}

MyString& operator=(const MyString& other)
{
    if (this == &other) // 1. Sprawdzenie samozadeklarowania
        return *this;

    // 2. Zwolnienie dotychczasowych zasobów
    delete[] data_;

    // 3. Kopiowanie rozmiaru i alokacja nowych zasobów
    size_ = other.size_;
    if (other.data_)
    {
        data_ = new char[size_ + 1];
        std::strcpy(data_, other.data_); // Głębokie kopiowanie danych
    }
    else { data_ = nullptr; }

    return *this; // 4. Zwrócenie odniesienia do siebie
}