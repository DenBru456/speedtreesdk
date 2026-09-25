#include "pch.hpp"
// stl_vectorized_compat.cpp
// Compatibility shim for VS2022 (v143) vectorized STL functions.
// Allows v143-compiled SpeedTreeRT static libs to link into v141_xp BigWorld builds.
// These are scalar fallbacks for the SIMD-optimized helpers that v143's
// compiler emits for std::find / std::find_last_of on trivial types.
// Functionally identical, just not SIMD-accelerated.

#include <cstddef>

extern "C"
{
    // ___std_find_trivial_4@12
    // std::find for 4-byte elements (used with CSpeedTreeRT* pointers on 32-bit).
    void* __stdcall __std_find_trivial_4(void* first, void* last, const void* value)
    {
        for (char* p = static_cast<char*>(first); p < static_cast<char*>(last); p += 4)
        {
            if (*reinterpret_cast<const void**>(p) == value)
                return p;
        }
        return last;
    }

    // ___std_find_last_of_trivial_pos_1@16
    // std::find_last_of for 1-byte elements (chars).
    unsigned int __stdcall __std_find_last_of_trivial_pos_1(
        const char* first, unsigned int first_count,
        const char* second, unsigned int second_count)
    {
        for (int i = static_cast<int>(first_count) - 1; i >= 0; --i)
        {
            for (unsigned int j = 0; j < second_count; ++j)
            {
                if (first[i] == second[j])
                    return static_cast<unsigned int>(i);
            }
        }
        return static_cast<unsigned int>(-1);
    }
}
