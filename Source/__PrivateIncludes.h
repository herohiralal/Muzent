#ifndef MZNT_PRIVATE_INLCUDES_H
#define MZNT_PRIVATE_INLCUDES_H

#include "Dependencies/Panshilar/Source/__PrivateIncludes.h"

/*
This is where we define what all rendering backends Muzent supports.
These can be turned off by defining the relevant macros to 0 before including
Muzent.
*/

#ifndef MZNT_VULKAN
    // TODO: add MoltenVK to support OSX/iOS
    #define MZNT_VULKAN (PNSLR_WINDOWS || PNSLR_LINUX || PNSLR_ANDROID)
#endif

#ifndef MZNT_DX12
    // TODO: implement
    #define MZNT_DX12 0 /*(PNSLR_WINDOWS)*/
#endif

#ifndef MZNT_METAL
    // TODO: implement
    #define MZNT_METAL 0 /*(PNSLR_APPLE)*/
#endif

PNSLR_SUPPRESS_WARN

// VULKAN
#if MZNT_VULKAN
    #if PNSLR_WINDOWS
        #define VK_USE_PLATFORM_WIN32_KHR
    #elif PNSLR_ANDROID
        #define VK_USE_PLATFORM_ANDROID_KHR
    #elif PNSLR_LINUX
        #define VK_USE_PLATFORM_XLIB_KHR
    #endif

    #ifndef __cplusplus
        #define VOLK_IMPLEMENTATION
    #endif
    #define VOLK_VULKAN_H_PATH "../vulkan/vulkan.h"
    #include "Dependencies/volk/volk.h"
    #undef VOLK_VULKAN_H_PATH

    #ifndef __cplusplus
        #define VMA_IMPLEMENTATION
    #endif
    #include "Dependencies/VMA/vk_mem_alloc.h"
    #undef VMA_IMPLEMENTATION
#endif

PNSLR_UNSUPPRESS_WARN

#endif//MZNT_PRIVATE_INCLUDES_H
