#include "__preprocessor__.h"
#include <iterator>

template <typename T>
struct entry_t
{
    T data;
    entry_t* next;
};

template <typename T>
class linked_list
{
    using entry = entry_t<T>;

    entry* head;
    entry* tail;
    size_t size;

    entry* new_entry(const T& _data)
    {
        size++;

        entry* e = new entry;
        e->next = nullptr;
        e->data = _data;
        return e;
    }

public:
    linked_list() : head(nullptr), tail(nullptr), size(0) {}

    void push_back(const T& _data)
    {
        entry* e = new_entry(_data);

        if (head == nullptr)
        {
            head = e;
            tail = e;
        }
        else
        {
            tail->next = e;
            tail = e;
        }
    }
    void push_front(const T& _data)
    {
        entry* e = new_entry(_data);

        if (head == nullptr)
        {
            head = e;
            tail = e;
        }
        else
        {
            e->next = head;
            head = e;
        }
    }

    void show_all()
    {
        int i = 0;

        // for (entry* e = head; e != nullptr; e = e->next)
        // {
        //     cout << i++ << ". " << e->data << endl;
        // }

        for (auto& e : *this)
        {
            cout << i++ << ". " << e << "\n";
        }

        cout << "\n";
    }
    T& operator[](size_t index)
    {
        if (size <= index) throw out_of_range("Index out of range");

        for (entry* e = head; e != nullptr; e = e->next)
        {
            if (index-- == 0) { return e->data; }
        }
    }

    // Definicja iteratora
    class iterator
    {
        entry* current;

    public:
        // Typy iteratora
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(entry* ptr) : current(ptr) {}

        // Przeciążenie operatora dereferencji
        reference operator*() const { return current->data; }
        pointer operator->() const { return &(current->data); }

        // Pre-inkrementacja
        iterator& operator++()
        {
            current = current->next;
            return *this;
        }

        // Post-inkrementacja
        iterator operator++(int)
        {
            iterator tmp = *this;
            current = current->next;
            return tmp;
        }

        // Operator porównania
        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return current != other.current; }
    };

    // Funkcje begin() i end() zwracające iteratory
    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }

    // Definicja const_iteratora
    class const_iterator
    {
        const entry* current;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator(const entry* ptr) : current(ptr) {}

        reference operator*() const { return current->data; }
        pointer operator->() const { return &(current->data); }

        const_iterator& operator++()
        {
            current = current->next;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            current = current->next;
            return tmp;
        }

        bool operator==(const const_iterator& other) const { return current == other.current; }
        bool operator!=(const const_iterator& other) const { return current != other.current; }
    };

    // Funkcje begin() i end() dla const_iteratora
    const_iterator begin() const { return const_iterator(head); }
    const_iterator end() const { return const_iterator(nullptr); }
};

void iterator_example()
{
    linked_list<int> l;
    l.push_back(10);
    l.push_back(20);
    l.push_back(30);
    l.push_back(40);
    l.push_back(50);

    l.show_all();

    for (const auto& e : l)
    {
        cout << e << endl;
    }

    cout << endl;

    for (linked_list<int>::iterator i = l.begin(); i != l.end(); ++i)
    {
        cout << *i << endl;
    }

    // cout << "Element at index 2: " << l[2] << endl;

    // try
    // {
    //     int index = 5;
    //     cout << "Element at index " << index << ". " << l[index] << endl;
    // }
    // catch (const out_of_range& e)
    // {
    //     cerr << "Error: " << e.what() << endl;
    // }
}