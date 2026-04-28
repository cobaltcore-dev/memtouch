#ifndef WRITE_PAGES_AVX512_HPP
#define WRITE_PAGES_AVX512_HPP

#include <cstdint>

void streaming_write_pages_avx512(void* mem_base, uint64_t page_start, uint64_t num_pages);

#endif
