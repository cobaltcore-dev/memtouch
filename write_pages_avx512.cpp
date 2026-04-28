#include <cstdint>
#include <cstdio>
#include <write_pages_avx512.hpp>
#include "consts.hpp"

// Dependents are responsible for checking that `avx512f` is available at runtime
#if defined(__x86_64__)
#include <immintrin.h>
void streaming_write_pages_avx512(void* mem_base, uint64_t page_start, uint64_t num_pages) {
    // Write 4 * 64 bytes per iteration until we are done
    // TODO: Compile time assert that PAGE_SIZE is indeed divisible by 64 * 4
    auto  num_iterations = num_pages * (PAGE_SIZE / (64 * 4));
    auto  pattern        = _mm512_set1_epi8(static_cast<int8_t>(PATTERN));
    char* mem_addr       = ((char*)mem_base) + (page_start * PAGE_SIZE);
    for (uint64_t n{0}; n < num_iterations; ++n) {
        _mm512_stream_si512(reinterpret_cast<__m512i*>(mem_addr), pattern);
        _mm512_stream_si512(reinterpret_cast<__m512i*>(mem_addr + 64), pattern);
        _mm512_stream_si512(reinterpret_cast<__m512i*>(mem_addr + (2 * 64)), pattern);
        _mm512_stream_si512(reinterpret_cast<__m512i*>(mem_addr + (3 * 64)), pattern);
        mem_addr += (4 * 64);
    }
    // Non-temporal stores are weakly ordered hence we apply a fence to ensure that
    // our stores are ordered before any subsequent (in program order) loads and stores.
    _mm_sfence();
}
#else
void streaming_write_pages_avx512(void* mem_base, uint64_t page_start, uint64_t num_pages) {
    (void)mem_base;
    (void)page_start;
    (void)num_pages;
    printf("this function should not be called\n");
    std::abort();
}
#endif
