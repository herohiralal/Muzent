#if defined(MZNT_IMPLEMENTATION) && MZNT_VULKAN && !defined(MZNT_VULKAN_PRIVATE_H)
#define MZNT_VULKAN_PRIVATE_H
#include "__Prelude.h"
#include "Renderer.h"
EXTERN_C_BEGIN

typedef struct MZNT_VulkanRenderer
{
    MZNT_Renderer    parent;
    VkInstance       instance;
    VkPhysicalDevice physicalDevice;
} MZNT_VulkanRenderer;

MZNT_VulkanRenderer* MZNT_CreateRenderer_Vulkan(MZNT_RendererConfiguration config);
b8 MZNT_DestroyRenderer_Vulkan(MZNT_VulkanRenderer* renderer);

EXTERN_C_END
#endif
