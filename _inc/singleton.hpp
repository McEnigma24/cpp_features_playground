#include "__preprocessor__.h"

template <typename T>
struct Singleton
{
    static T& getInstance()
    {
        static T instance;
        return instance;
    }
};

class test : public Singleton<test>
{
public:
    test() { line("test Constructor called"); }
    ~test() { line("test Destructor called"); }
};

void singletining()
{
    line("Singleton test - start");

    test& instance1 = test::getInstance();
    test& instance2 = test::getInstance();
    test& instance3 = test::getInstance();

    line("Singleton test - end");
}