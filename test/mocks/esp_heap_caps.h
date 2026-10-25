#pragma once

#include <cstddef>
#include <cstdint>

constexpr uint32_t MALLOC_CAP_DEFAULT = 0;

struct multi_heap_info_t {
    std::size_t total_free_bytes = 8U * 1024U * 1024U;
    std::size_t total_allocated_bytes = 0;
    std::size_t largest_free_block = 8U * 1024U * 1024U;
    std::size_t minimum_free_bytes = 8U * 1024U * 1024U;
    std::size_t allocated_blocks = 0;
    std::size_t free_blocks = 1;
    std::size_t total_blocks = 1;
};

inline void heap_caps_get_info(multi_heap_info_t* info, uint32_t){
    if(info){
        *info = multi_heap_info_t {};
    }
}
