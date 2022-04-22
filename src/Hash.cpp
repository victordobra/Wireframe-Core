#include "Hash.hpp"
#include "string.hpp"

namespace mge {
    constexpr uint32_t LARGEST_PRIME = 4294967291u;

    template<>
    void GetHashCode(sint8_t val, size_t& hash) {
        hash = (size_t)val;
    }
    template<>
    void GetHashCode(sint16_t val, size_t& hash) {
        hash = (size_t)val;
    }
    template<>
    void GetHashCode(sint32_t val, size_t& hash) {
        hash = (size_t)val;
    }
    template<>
    void GetHashCode(sint64_t val, size_t& hash) {
#ifdef ARCH_x86
        hash = (size_t)(val % LARGEST_PRIME);
#else
        hash = (size_t)val;
#endif
    }
    template<>
    void GetHashCode(uint8_t val, size_t& hash) {
        hash = (size_t)val;
    }
    template<>
    void GetHashCode(uint16_t val, size_t& hash) {
        hash = (size_t)val;
    }
    template<>
    void GetHashCode(uint32_t val, size_t& hash) {
        hash = (size_t)val;
    }
    template<>
    void GetHashCode(uint64_t val, size_t& hash) {
#ifdef ARCH_x86
        hash = (size_t)(val % LARGEST_PRIME);
#else
        hash = (size_t)val;
#endif
    }
    template<>
    void GetHashCode(float32_t val, size_t& hash) {
        uint32_t target;
        memcpy(&target, &val, sizeof(float32_t));

        GetHashCode(target, hash);
    }
    template<>
    void GetHashCode(float64_t val, size_t& hash) {
        uint64_t target;
        memcpy(&target, &val, sizeof(float64_t));

        GetHashCode(target, hash);
    }
    template<>
    void GetHashCode(string val, size_t& hash) {
        hash = 0x811c9dc5;
        const size_t prime = 0x1000193;

        for (int i = 0; i < val.length(); i++) {
            uint8_t value = val[i];
            hash = hash ^ value;
            hash *= prime;
        }
    }
}