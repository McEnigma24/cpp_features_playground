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

    const std::span<const string_type> get_names() const { return names; }
};

int main()
{
    line("nothing yet...");

    Test t{.a = 1, .b{2}, .c{3}};

    cout << t.a << " " << t.b << " " << t.c << endl;

    TestUnion t1{.a = 10};
    TestUnion t2{.b = 3.14f};
    TestUnion t3{.c = 'a'};

    line("before NamesList Constructor");
    NamesList nl{{"John", "Jane", "Jim"}};
    line("after NamesList Constructor");

    line("NamesList get_names");
    for (auto& name : nl.get_names())
    {
        cout << name << endl;
    }
}