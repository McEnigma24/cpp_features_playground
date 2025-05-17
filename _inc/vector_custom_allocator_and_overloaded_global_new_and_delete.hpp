#include "__preprocessor__.h"

void* operator new(size_t size)
{
    cout << "New operator overloading " << endl;
    void* p = malloc(size);
    return p;
}

void operator delete(void* p)
{
    cout << "Delete operator overloading " << endl;
    free(p);
}

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

void zuza(int liczba_calkowita) { int wiek_zuzy = liczba_calkowita; }

// Custom allocator that uses a pre-allocated buffer
template <typename T>
class BufferAllocator
{
public:
    using value_type = T;

    // Constructor takes existing buffer and its capacity (in elements)
    BufferAllocator(T* buffer_ptr, std::size_t capacity) : buffer(buffer_ptr), capacity(capacity), offset(0) {}

    // Copy constructor template for rebinding
    template <typename U>
    BufferAllocator(const BufferAllocator<U>& other) noexcept : buffer(other.buffer), capacity(other.capacity), offset(other.offset)
    {
    }

    // Allocate n elements from the buffer
    T* allocate(std::size_t n)
    {
        if (offset + n > capacity) { throw std::bad_alloc(); }
        T* ptr = buffer + offset;
        offset += n;
        return ptr;
    }

    // Deallocate does nothing (buffer persists)
    void deallocate(T* /*p*/, std::size_t /*n*/) noexcept
    {
        // no-op
    }

    // Allocator traits
    template <typename U>
    struct rebind
    {
        using other = BufferAllocator<U>;
    };

private:
    template <typename U>
    friend class BufferAllocator;

    T* buffer;
    std::size_t capacity;
    std::size_t offset;
};

void vector_custom_allocator_and_overloaded_global_new_and_delete()
{
    time_stamp("vector_custom_allocator_and_overloaded_global_new_and_delete");
    // checking_if_it_works();

    for (;;)
    {
        const size_t BUFFER_SIZE = 100000000;

        vector<int> vec(BUFFER_SIZE);
        memset(vec.data(), 66, BUFFER_SIZE * sizeof(int));
        var(vec[0]);
    }

    // int main() {
    //     constexpr std::size_t BUFFER_SIZE = 100;
    //     // Pre-allocated storage for 100 ints
    //     static int preallocated_buffer[BUFFER_SIZE];

    //     // Create vector using our custom allocator
    //     std::vector<int, BufferAllocator<int>> vec(
    //         BufferAllocator<int>(preallocated_buffer, BUFFER_SIZE)
    //     );

    //     // Use the vector as usual
    //     for (int i = 0; i < 10; ++i) {
    //         vec.push_back(i * i);
    //     }

    //     // Output contents
    //     for (int x : vec) {
    //         std::cout << x << " ";
    //     }
    //     std::cout << std::endl;

    //     return 0;
    // }
}