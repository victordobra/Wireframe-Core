#pragma once

#include "Defines.hpp"

namespace mge {
    template<class T>
    void GetHashCode(T val, size_t& hash);
    
    template<class T>
    inline size_t GetHashCode(T val) { 
        size_t hash = 0; 
        GetHashCode(val, hash); 
        return hash; 
    }
}