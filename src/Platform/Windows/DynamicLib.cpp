#include "DynamicLib.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace wfe {
    bool8_t DynamicLib::LoadLib(const char_t* name) {
        // Check if a dynamic lib has already been loaded
        if(internalData)
            return false;
        
        // Try to load the dynamic lib using LoadLibraryA
        internalData = (void*)LoadLibraryA(name);

        return internalData;
    }
    DynamicLib::VoidFunction DynamicLib::LoadFunc(const char_t* name) {
        // Check if no dynamic lib has been loaded
        if(!internalData)
            return nullptr;

        // Try to load the function using GetProcAddress
        return (VoidFunction)GetProcAddress((HMODULE)internalData, name);
    }
    void DynamicLib::FreeLib() {
        // Exit the function if the dynamic lib has been loaded
        if(!internalData)
            return;
        
        // Free the dynamic lib using FreeLibrary
        FreeLibrary((HMODULE)internalData);
    }

    DynamicLib::~DynamicLib() {
        // Try to free the dynamic lib
        FreeLib();
    }
}