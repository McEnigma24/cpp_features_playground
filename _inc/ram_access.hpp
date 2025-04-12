#include "__preprocessor__.h"
#include <immintrin.h>
#include <mm_malloc.h>

void ram_access()
{
    const size_t N = 1024 * 1024 * 1000; // 1MB
    // int* a = new int[N];
    int* a = (int*)_mm_malloc(sizeof(int) * N, 32);
    // int* a = nullptr;
    // if(posix_memalign((void**)&a, 32, sizeof(int) * N) != 0) {
    //     // obsługa błędu alokacji
    //     return;
    // }
    memset(a, 0, sizeof(int) * N);

    time_stamp_reset();

    const __m256i zeros = _mm256_set1_epi32(0);

    for (int i = 0; i + 7 < N; i += 8)
        _mm256_stream_si256((__m256i*)&a[i], zeros);
    time_stamp("stream");

    for (int i = 0; i + 7 < N; i += 8)
        _mm256_store_si256((__m256i*)&a[i], zeros);
    time_stamp("store");
}

void no_write_allocate_memcpy(void* dest, const void* src, size_t n)
{
    // Wymagane: dest powinno być wyrównane do 32 bajtów dla efektywnego użycia streaming store.
    char* dst = reinterpret_cast<char*>(dest);
    const char* source = reinterpret_cast<const char*>(src);

    // Ustal, czy adres docelowy jest wyrównany do 32 bajtów.
    uintptr_t dst_addr = reinterpret_cast<uintptr_t>(dst);
    size_t misalign = dst_addr % 32;
    size_t head = 32 - misalign;

    // Jeśli mamy początkowy fragment do wyrównania, skopiuj go standardowo
    if (head > 0 && head <= n)
    {
        std::memcpy(dst, source, head);
        dst += head;
        source += head;
        n -= head;
    }

    // Kopiowanie głównej części – pełnych bloków 32-bajtowych
    size_t chunks = n / 32;
    for (size_t i = 0; i < chunks; i++)
    {
        // Ładujemy 32 bajty z pamięci źródłowej, nie martwiąc się o wyrównanie
        __m256i data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(source));
        // Strumieniowo zapisujemy dane – wymagana jest wyrównana dest
        _mm256_stream_si256(reinterpret_cast<__m256i*>(dst), data);
        dst += 32;
        source += 32;
    }

    // Opcjonalnie, kopiujemy końcowy fragment, który jest mniejszy niż 32 bajty
    size_t tail = n % 32;
    if (tail > 0) { std::memcpy(dst, source, tail); }

    // Zapewnij, że wszystkie operacje strumieniowe zostaną zakończone
    _mm_sfence();
}
