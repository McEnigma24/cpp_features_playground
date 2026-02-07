#include "__preprocessor__.h"

#include <stack>

template <typename container_t>
struct prePopRef
{
    container_t& container;

    prePopRef(container_t& container) : container(container) {}

    container_t::value_type& get() { return container.top(); }

    // auto& get()
    // {
    //     return container.top();
    // }

    ~prePopRef() { container.pop(); }
};

int main()
{
    stack<char> input_bytes;

    input_bytes.push('a');
    input_bytes.push('b');
    input_bytes.push('c');
    input_bytes.push('d');

    {
        prePopRef pr(input_bytes);
        auto& front = pr.get();

        cout << front << endl;
    }

    return 0;
}
