#include "__preprocessor__.h"

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

int main()
{
    line("nothing yet...");

    Test t{.a = 1, .b{2}, .c{3}};

    cout << t.a << " " << t.b << " " << t.c << endl;

    TestUnion t1{.a = 10};
    TestUnion t2{.b = 3.14f};
    TestUnion t3{.c = 'a'};
}