#include "__preprocessor__.h"
#include <concepts>

// Dynamic //

int global_var = 0;

class IDynamic
{
public:
    virtual void process() const = 0;
};

class Dynamic_imp_1 : public IDynamic
{
public:
    virtual void process() const override
    {
        // std::cout << "Dynamic_imp_1: wykonuję operację." << std::endl;
        global_var++;
    }
};

class Dynamic_imp_2 : public IDynamic
{
public:
    virtual void process() const override
    {
        // std::cout << "Dynamic_imp_2: wykonuję operację." << std::endl;
        global_var++;
    }
};

class User_dynamic
{
    IDynamic* m_impl;

public:
    User_dynamic(IDynamic* impl) : m_impl(impl) {}

    void process() const { m_impl->process(); }
};

// Static //

// clang-format off
template <typename T>
concept IStatic = requires(T t)
{
    { t.process() } -> std::same_as<void>;
};
// clang-format on

class Static_imp_1
{
public:
    void process() const
    {
        // std::cout << "Static_imp_1: wykonuję operację." << std::endl;
        global_var++;
    }
};
static_assert(IStatic<Static_imp_1>);

class Static_imp_2
{
public:
    void process() const
    {
        // std::cout << "Static_imp_2: wykonuję operację." << std::endl;
        global_var++;
    }
};
static_assert(IStatic<Static_imp_2>);

template <IStatic imp_t>
class User_static
{
    imp_t* m_impl;

public:
    User_static(imp_t* impl) : m_impl(impl) {}

    void process() const { m_impl->process(); }
};

int main()
{
    User_static<Static_imp_1> user_static_1(new Static_imp_1());

    // User_static<Static_imp_1> user_static_1(new Static_imp_2());    // Error
    // User_static<IStatic> user_static_1(new Static_imp_1());         // Error

    User_dynamic user_dynamic_1(new Dynamic_imp_1());
    size_t max = 1000000;

    time_stamp_reset();

    for (int i = 0; i < max; i++)
    {
        user_static_1.process();
    }
    time_stamp("Static");

    for (int i = 0; i < max; i++)
    {
        user_dynamic_1.process();
    }
    time_stamp("Dynamic");
}