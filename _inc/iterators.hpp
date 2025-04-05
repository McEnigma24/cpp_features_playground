#include "__preprocessor__.h"
#include <iterator>

template <typename T>
struct entry
{
    T data;
    entry* next;
}

template <typename T>
class linked_list
{
    entry<T>* head;
    entry<T>* tail;
    size_t size;

public:
    linked_list() : head(nullptr), tail(nullptr), size(0) {}

    push_back()
    {
        if (head == nullptr)
        {
            head = new entry<T>;
            head->next = nullptr;
            head->data = T();
            tail = head;
        }
        else
        {
            tail->next = new entry<T>;
            tail->next->next = nullptr;
            tail->next->data = T();
            tail = tail->next;
        }
    }

    push_front() {}
}





void iterator()
{
}