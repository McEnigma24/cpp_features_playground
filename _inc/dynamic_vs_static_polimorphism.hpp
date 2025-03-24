#include "__preprocessor__.h"
#include <concepts>

// Definicja konceptu, który wymaga istnienia metody process() zwracającej void.
template <typename T>
concept Processable = requires(T t)
{
    {
        t.process()
        } -> std::same_as<void>;
};

// Pierwsza implementacja spełniająca koncept Processable.
class Implementation1
{
public:
    void process() const { std::cout << "Implementation1: wykonuję operację." << std::endl; }
};

// Druga implementacja spełniająca koncept Processable.
class Implementation2
{
public:
    void process() const { std::cout << "Implementation2: wykonuję operację." << std::endl; }
};

// Funkcja szablonowa, która przyjmuje obiekt spełniający koncept Processable.
// Dzięki temu wybór implementacji następuje w czasie kompilacji.
template <Processable T>
void runProcess(const T& obj)
{
    obj.process();
}

int main()
{
    Implementation1 impl1;
    Implementation2 impl2;

    runProcess(impl1); // Wywoła Implementation1::process()
    runProcess(impl2); // Wywoła Implementation2::process()

    return 0;
}