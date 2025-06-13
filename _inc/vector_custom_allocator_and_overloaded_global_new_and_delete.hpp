#include "__preprocessor__.h"

static size_t currently_used_memory_in_BYTES = 0;
static size_t last_allocated_size = 0;

// void* operator new(size_t size)
// {
//     cout << "New operator overloading - " << size << endl;
//     last_allocated_size = size;
//     currently_used_memory_in_BYTES += size;
//     sizeof_imp(currently_used_memory_in_BYTES);
//     void* p = malloc(size);
//     return p;
// }

// void operator delete(void* p)
// {
//     cout << "Delete operator overloading - NO SIZE" << endl;
//     currently_used_memory_in_BYTES -= last_allocated_size;
//     sizeof_imp(currently_used_memory_in_BYTES);
//     free(p);
// }

void checking_if_it_works()
{
    int* p = new int[10];
    int* pa = new int[10];
    int* paa = new int[10];
    int* paaa = new int[10];
    int* paaaa = new int[10];
    int* paaaaa = new int[10];
    int* paaaaaa = new int[10];
    int* paaaaaaa = new int[10];

    var(p[0]);
    var(pa[0]);
    var(paa[0]);
    var(paaa[0]);
    var(paaaa[0]);
    var(paaaaa[0]);
    var(paaaaaa[0]);
    var(paaaaaaa[0]);

    delete p;
    delete pa;
    delete paa;
    delete paaa;
    delete paaaa;
    delete paaaaa;
    delete paaaaaa;
    delete paaaaaaa;
}

template <typename T>
class BufferAllocator
{
public:
    using value_type = T;

    // Constructor takes existing buffer and its capacity (in elements)
    BufferAllocator(T* buffer, size_t capacity) : buffer(buffer), capacity(capacity) {}

    // Allocate n elements from the buffer
    T* allocate(size_t n)
    {
        if (n > capacity) { throw bad_alloc(); }
        return buffer;
    }

    void deallocate(T* /*p*/, size_t /*n*/) noexcept { delete[] buffer; }

private:
    T* buffer;
    size_t capacity;
};

void vector_custom_allocator_and_overloaded_global_new_and_delete()
{
    time_stamp("vector_custom_allocator_and_overloaded_global_new_and_delete");
    // checking_if_it_works();

    const size_t BUFFER_SIZE = 100'000'000;

    for (;;)
    {
        int* buffer = new int[BUFFER_SIZE];

        // Create vector using our custom allocator //

        //                                        n                                       capacity
        vector<int, BufferAllocator<int>> vec(BUFFER_SIZE, BufferAllocator<int>(buffer, BUFFER_SIZE));

        memset(vec.data(), -1, BUFFER_SIZE * sizeof(int));
        memset(buffer, -1, BUFFER_SIZE * sizeof(int));
    }
}