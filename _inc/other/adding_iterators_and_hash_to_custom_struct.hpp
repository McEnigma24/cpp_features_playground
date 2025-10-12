#include "__preprocessor__.h"
#include <functional>
#include <iterator>
#include <unordered_map>

struct test_struct
{
    int tab[10];

    test_struct(int starting_point = 0)
    {
        for (int i = 0; i < 10; i++)
        {
            tab[i] = (starting_point + i) % 10;
        }
    }

    // Hash //
    bool operator==(const test_struct& other) const { return tab == other.tab; }

    // Deklaracja funkcji zaprzyjaźnionej do przeciążenia operatora <<
    friend ostream& operator<<(ostream& os, const test_struct& entry)
    {
        for (int i = 0; i < 10; i++)
        {
            os << entry.tab[i] << " ";
        }
        return os;
    }
};

// Specjalizacja hash dla test_struct //  -> potem gdzieś to jest wołane jako hash("obiekt naszej klasy")
//                                            operator(), np. Multiply mul; float result = mul(2, 3);
namespace std
{
    template <>
    struct hash<test_struct>
    {
        size_t operator()(const test_struct& entry) const
        {
            //                 has to have "()"
            return hash<const void*>()(static_cast<const void*>(entry.tab));
        }
    };
} // namespace std

template <typename T>
struct entry_t
{
    T data;
    entry_t* next;

    // Hash //
    bool operator==(const entry_t& other) const { return data == other.data; }

    // Deklaracja funkcji zaprzyjaźnionej do przeciążenia operatora <<
    friend ostream& operator<<(ostream& os, const entry_t& entry)
    {
        os << entry.data;
        return os;
    }
};

// Specjalizacja hash dla entry_t<T> //
namespace std
{
    template <typename T>
    struct hash<entry_t<T>>
    {
        size_t operator()(const entry_t<T>& entry) const
        {
            // Używamy hash dla typu T (zakładamy, że T jest hashowalny)
            return hash<T>()(entry.data);
        }
    };
} // namespace std

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

        for (entry* e = head; e != nullptr; e = e->next)
        {
            cout << i++ << ". " << e->data << endl;
        }

        // for (auto& e : *this)
        // {
        //     cout << i++ << ". " << e << "\n";
        // }

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
        using iterator_category = forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
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
        using iterator_category = forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
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
    const_iterator begin_const() const { return const_iterator(head); }
    const_iterator end_const() const { return const_iterator(nullptr); }
};

void normal_hash()
{
    unordered_map<string, int> hash_map;
    hash_map["key1"] = 1;
    hash_map["key2"] = 2;
    hash_map["key3"] = 3;
    hash_map["key4"] = 4;
    hash_map["key5"] = 5;

    for (const auto& pair : hash_map)
    {
        cout << pair.first << ": " << pair.second << "\n";
    }
    cout << "\n";

    for (const auto& [key, value] : hash_map)
    {
        cout << key << ": " << value << "\n";
    }
    cout << "\n";
}

void hash_example()
{
    {
        // unordered_map<entry_t<int>, string> hash_map;
        // hash_map[{ 1, nullptr }] = "key1";
        // hash_map[{ 2, nullptr }] = "key2";

        // for (const auto& [key, value] : hash_map)
        // {
        //     cout << key << ": " << value << "\n";
        // }cout << "\n";
    }

    {
        // var(1 % 10);
        // var(0 % 10);
        // var((-1 + 10) % 10);
        // var((-2 + 10) % 10);
    }

    {
        // // Does not need hash function //
        // unordered_map<string, test_struct> hash_map;
        // hash_map["key1"] = { };
        // hash_map["key2"] = { 1 };

        // for (const auto& [key, value] : hash_map)
        // {
        //     cout << key << ": " << value << "\n";
        // }cout << "\n";
    }

    {
        // NEEDs hash function //
        unordered_map<test_struct, string> hash_map;
        hash_map[{0}] = "key1";
        hash_map[{1}] = "key2";

        for (const auto& [key, value] : hash_map)
        {
            cout << key << ": " << value << "\n";
        }
        cout << "\n";
    }
}

int main()
{
    linked_list<int> l;
    l.push_back(10);
    l.push_back(20);
    l.push_back(30);
    l.push_back(40);
    l.push_back(50);

    l.show_all();

    line("for (auto& e : l)");
    for (auto& e : l)
    {
        cout << e++ << endl;
    }
    cout << endl;

    line("for (const auto& e : l)");
    for (const auto& e : l)
    {
        cout << e << endl;
    }
    cout << endl;

    line("for (linked_list<int>::iterator i = l.begin(); i != l.end(); ++i)");
    for (linked_list<int>::iterator i = l.begin(); i != l.end(); ++i)
    {
        cout << *i << endl;

        (*i)++;
    }
    cout << endl;

    line("for (linked_list<int>::const_iterator i = l.begin_const(); i != l.end_const(); ++i)");
    for (linked_list<int>::const_iterator i = l.begin_const(); i != l.end_const(); ++i)
    {
        cout << *i << endl;

        // (*i)++; // Error
    }
    cout << endl;

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