#include "__preprocessor__.h"

namespace XorList
{
    template <typename T>
    struct node
    {
        T value;
        node* pBoth;

        node(const node* pPrev, const node* pNext)
            : value(T()), pBoth(reinterpret_cast<node*>(reinterpret_cast<uintptr_t>(pPrev) ^ reinterpret_cast<uintptr_t>(pNext)))
        {
        }
    };

    template <typename T>
    class XorLinkedList
    {
        using node = XorList::node<T>;

        size_t size;
        node* head;
        node* tail;

    public:
        XorLinkedList() : head(nullptr), tail(nullptr) {}

        void push_front(const T& value)
        {
            if (head == nullptr)
            {
                head = new node(nullptr, nullptr);
                head->value = value;
                tail = head;
            }
            else
            {
                node* newNode = new node(nullptr, head);
                head->pBoth = reinterpret_cast<node*>(reinterpret_cast<uintptr_t>(head->pBoth) ^ reinterpret_cast<uintptr_t>(newNode));
                head = newNode;
                head->value = value;
            }
            size++;
        }

        void push_back(const T& value)
        {
            if (head == nullptr)
            {
                head = new node(nullptr, nullptr);
                head->value = value;
                tail = head;
            }
            else
            {
                node* newNode = new node(tail, nullptr);
                tail->pBoth = reinterpret_cast<node*>(reinterpret_cast<uintptr_t>(tail->pBoth) ^ reinterpret_cast<uintptr_t>(newNode));
                tail = newNode;
                tail->value = value;
            }
            size++;
        }

        void go_forward()
        {
            node* current = head;
            node* prev = nullptr;

            while (current != nullptr)
            {
                // varr(current);
                var(current->value);

                node* next = reinterpret_cast<node*>(reinterpret_cast<uintptr_t>(prev) ^ reinterpret_cast<uintptr_t>(current->pBoth));
                prev = current;
                current = next;
            }
            std::cout << std::endl;
        }

        void go_backward()
        {
            node* current = tail;
            node* next = nullptr;

            while (current != nullptr)
            {
                // varr(current);
                var(current->value);

                node* prev = reinterpret_cast<node*>(reinterpret_cast<uintptr_t>(next) ^ reinterpret_cast<uintptr_t>(current->pBoth));
                next = current;
                current = prev;
            }
            std::cout << std::endl;
        }
    };
} // namespace XorList

using namespace XorList;

void xoiring()
{
    XorList::XorLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(2);
    list.push_back(2);
    list.push_back(3);

    list.push_front(0);
    list.push_front(-1);
    list.push_front(-2);

    list.go_forward();
    list.go_backward();
}