#include "__preprocessor__.h"

#include <iostream>
#include <utility> // dla std::move

class MyBuffer
{
    int* data;
    size_t size;

public:
    // Konstruktor podstawowy — alokuje zasób
    MyBuffer(size_t n) : data(new int[n]), size(n) { std::cout << "Constructor: allocate " << size << "\n"; }

    // Kopiujący konstruktor
    MyBuffer(const MyBuffer& other) : data(new int[other.size]), size(other.size)
    {
        std::cout << "Copy constructor: copy " << size << "\n";
        for (size_t i = 0; i < size; ++i)
            data[i] = other.data[i];
    }

    // Operator przenoszący
    MyBuffer& operator=(const MyBuffer& other)
    {
        if (this != &other)
        {
            delete[] data;

            data = new int[other.size];
            size = other.size;

            memcpy(data, other.data, size * sizeof(int));
        }
        return *this;
    }

    // Konstruktor przenoszący
    MyBuffer(MyBuffer&& other) noexcept
        : data(other.data), size(other.size) // kradniemy buffer innego obiektu i zabieramy mu możliwość zarządzaniam tymi danymi
    {
        std::cout << "Move constructor: steal " << size << "\n";
        other.data = nullptr;
        other.size = 0;
    }

    // Operator przenoszący
    MyBuffer& operator=(MyBuffer&& other) noexcept
    {
        if (this != &other)
        {
            // zwolnij obecne zasoby
            delete[] data;

            // „skradnij” zasoby z other
            data = other.data;
            size = other.size;

            // pozostaw other w stanie „pustym”
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // Destruktor
    ~MyBuffer()
    {
        if (data) { std::cout << "Destructor: delete " << size << "\n"; }
        else { std::cout << "Destructor: nothing to delete\n"; }
        delete[] data;
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
