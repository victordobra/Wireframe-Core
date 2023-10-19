#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_LINUX
#include "DynamicLib.hpp"
#include <dlfcn.h>

namespace wfe {
    bool8_t DynamicLib::LoadLib(const char_t* name) {
        // Check if a dynamic lib has already been loaded
        if(internalData)
            return false;
        
        // Try to load the library using dlopen
        internalData = dlopen(name, RTLD_NOW);

        return internalData;
    }
    DynamicLib::VoidFunction DynamicLib::LoadFunc(const char_t* name) {
        // Check if no dynamic lib has been loaded
        if(!internalData)
            return nullptr;
        
        // Try to load the function using dlsym
        return (VoidFunction)dlsym(internalData, name);
    }
    void DynamicLib::FreeLib() {
        // Check if no dynamic lib has been loaded
        if(!internalData)
            return;

        // Free the dynamic lib using dlclose
        dlclose(internalData);
    }

    DynamicLib::~DynamicLib() {
        // Try to free the dynamic lib
        FreeLib();
    }
}

#endif