#include "__preprocessor__.h"

#include <iostream>
#include <utility> // dla std::move

class MyBuffer
{
    int* data_;
    size_t size_;

public:
    // Konstruktor podstawowy — alokuje zasób
    MyBuffer(size_t n) : data_(new int[n]), size_(n) { std::cout << "Constructor: allocate " << size_ << "\n"; }

    // Kopiujący konstruktor
    MyBuffer(const MyBuffer& other) : data_(new int[other.size_]), size_(other.size_)
    {
        std::cout << "Copy constructor: copy " << size_ << "\n";
        for (size_t i = 0; i < size_; ++i)
            data_[i] = other.data_[i];
    }

    // Konstruktor przenoszący
    MyBuffer(MyBuffer&& other) noexcept
        : data_(other.data_), size_(other.size_) // kradniemy buffer innego obiektu i zabieramy mu możliwość zarządzaniam tymi danymi
    {
        std::cout << "Move constructor: steal " << size_ << "\n";
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // Operator przenoszący
    MyBuffer& operator=(MyBuffer&& other) noexcept
    {
        if (this != &other)
        {
            // zwolnij obecne zasoby
            delete[] data_;

            // „skradnij” zasoby z other
            data_ = other.data_;
            size_ = other.size_;

            // pozostaw other w stanie „pustym”
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // Destruktor
    ~MyBuffer()
    {
        if (data_) { std::cout << "Destructor: delete " << size_ << "\n"; }
        else { std::cout << "Destructor: nothing to delete\n"; }
        delete[] data_;
    }
};

MyBuffer make_buffer(size_t n)
{
    return MyBuffer(n); // (1) tworzymy tymczasowy obiekt i zwracamy
}

int main()
{
    MyBuffer buf1 = make_buffer(5);
    // W tym miejscu konstruktor przenoszący może zostać użyty
    // (lub zoptymalizowany przez elizję)

    MyBuffer buf2(10);
    buf2 = MyBuffer(3); // tu również może się włączyć przenoszenie

    MyBuffer buf3(7);
    // Wymuszenie przenoszenia:
    buf3 = std::move(buf2);

    return 0;
}
