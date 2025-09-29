#define MZNT_IMPLEMENTATION
#include "RendererPrivate.h"
#if MZNT_VULKAN

MZNT_VulkanRenderer* MZNT_CreateRenderer_Vulkan(MZNT_RendererConfiguration config)
{
    MZNT_VulkanRenderer* output = PNSLR_New(MZNT_VulkanRenderer, config.allocator, PNSLR_GET_LOC(), nil);

    volkInitialize();

    return output;
}

b8 MZNT_DestroyRenderer_Vulkan(MZNT_VulkanRenderer* renderer)
{
    if (!renderer) return false;

    volkFinalize();

    PNSLR_Delete(renderer, renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    return true;
}

#endif
