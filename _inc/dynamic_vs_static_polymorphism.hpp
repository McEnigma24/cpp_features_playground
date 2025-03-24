#include "__preprocessor__.h"
#include <concepts>

// Dynamic //

class Dynamic_Base
{
public:
    virtual void process() const = 0;
};

class Dynamic_imp_1 : public Dynamic_Base
{
public:
    virtual void process() const override { std::cout << "Dynamic_imp_1: wykonuję operację." << std::endl; }
};

class Dynamic_imp_2 : public Dynamic_Base
{
public:
    virtual void process() const override { std::cout << "Dynamic_imp_2: wykonuję operację." << std::endl; }
};

// Static // // clang-format off

template <typename T>
concept Static_base = requires(T t)
{
    {
        t.process()
        } -> std::same_as<void>;
};
// clang-format on

class Static_imp_1
{
public:
    void process() const { std::cout << "Static_imp_1: wykonuję operację." << std::endl; }
};
static_assert(Static_base<Static_imp_1>);

class Static_imp_2
{
public:
    void process() const { std::cout << "Static_imp_2: wykonuję operację." << std::endl; }
};
static_assert(Static_base<Static_imp_2>);

template <Static_base T>
void runProcess(const T& obj)
{
    obj.process();
}

int main_dynamic_static_polymorphism()
{
    Static_imp_1 impl1;
    Static_imp_2 impl2;

    runProcess(impl1);
    runProcess(impl2);

    return 0;
}