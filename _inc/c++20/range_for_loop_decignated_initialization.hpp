#include "__preprocessor__.h"
#include <span>

struct Test
{
    int a, b, c;
};

union TestUnion
{
    char c;
    int a;
    float b;
};

class MyString
{
    char* data;
    size_t size;

public:
    MyString(const char* _data) : data(new char[strlen(_data) + 1])
    {
        line("MyString Constructor");
        strcpy(data, _data);
        size = strlen(data);
    }

    // Copy constructor
    MyString(const MyString& other) : data(new char[other.size + 1]), size(other.size)
    {
        line("MyString Copy Constructor");
        strcpy(data, other.data);
    }

    // Copy assignment operator
    MyString& operator=(const MyString& other)
    {
        line("MyString Copy Assignment");
        if (this != &other)
        {
            delete[] data;
            size = other.size;
            data = new char[size + 1];
            strcpy(data, other.data);
        }
        return *this;
    }

    // Move constructor
    MyString(MyString&& other) noexcept : data(other.data), size(other.size)
    {
        line("MyString Move Constructor");
        other.data = nullptr;
        other.size = 0;
    }

    // Move assignment operator
    MyString& operator=(MyString&& other) noexcept
    {
        line("MyString Move Assignment");
        if (this != &other)
        {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    ~MyString()
    {
        line("MyString Destructor");
        delete[] data;
    }

    char* get_data() const { return data; }
};

std::ostream& operator<<(std::ostream& os, const MyString& str)
{
    os << str.get_data();
    return os;
}

typedef std::string string_type;
// typedef MyString string_type;

class NamesList
{
    std::vector<string_type> names;

public:
    explicit NamesList(std::vector<string_type>&& _names) : names(std::move(_names))
    {
        line("NamesList Constructor");
        for (auto& name : names)
        {
            cout << name << endl;
        }
    }

    std::vector<string_type>& get_names() { return names; }
};

NamesList create() { return NamesList{{"John", "Jane", "Jim"}}; }

int main()
{
    line("nothing yet...");

    Test t{.a = 1, .b{2}, .c{3}};

    cout << t.a << " " << t.b << " " << t.c << endl;

    TestUnion t1{.a = 10};
    TestUnion t2{.b = 3.14f};
    TestUnion t3{.c = 'a'};

    // for (auto& name : create().get_names()) // this expands to
    // {
    //     cout << name << endl;
    // }

    for (auto nameList = create(); auto& name : nameList.get_names())
    {
        cout << name << endl;
    }
}