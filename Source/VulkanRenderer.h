#if defined(MZNT_IMPLEMENTATION) && MZNT_VULKAN && !defined(MZNT_VULKAN_PRIVATE_H)
#define MZNT_VULKAN_PRIVATE_H
#include "__Prelude.h"
#include "Renderer.h"
EXTERN_C_BEGIN

PNSLR_DECLARE_ARRAY_SLICE(VkPhysicalDevice);
PNSLR_DECLARE_ARRAY_SLICE(VkQueueFamilyProperties);
PNSLR_DECLARE_ARRAY_SLICE(VkDeviceQueueCreateInfo);
PNSLR_DECLARE_ARRAY_SLICE(VkSurfaceFormatKHR);
PNSLR_DECLARE_ARRAY_SLICE(VkImage);
PNSLR_DECLARE_ARRAY_SLICE(VkImageView);

typedef struct MZNT_VulkanRenderer
{
    MZNT_Renderer    parent;
    VkInstance       instance;
    #if PNSLR_DBG
        VkDebugUtilsMessengerEXT debugMessenger;
    #endif
    VkPhysicalDevice physicalDevice;
    VkDevice         device;
    u32              gfxQueueFamilyIndex;
    u32              presQueueFamilyIndex;
    VkQueue          gfxQueue;
    VkQueue          presQueue;
} MZNT_VulkanRenderer;

MZNT_VulkanRenderer* MZNT_CreateRenderer_Vulkan(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRenderer_Vulkan(MZNT_VulkanRenderer* renderer, PNSLR_Allocator tempAllocator);

typedef struct MZNT_VulkanRendererSurface
{
    MZNT_RendererSurface          parent;
    MZNT_VulkanRenderer*          renderer;
    VkSurfaceKHR                  surface;
    VkSwapchainKHR                swapchain;
    PNSLR_ArraySlice(VkImage)     swapchainImages;
    PNSLR_ArraySlice(VkImageView) swapchainImageViews;
} MZNT_VulkanRendererSurface;

MZNT_VulkanRendererSurface* MZNT_CreateRendererSurface_Vulkan(MZNT_VulkanRenderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator);
b8 MZNT_ResizeRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator);

typedef struct MZNT_VulkanRendererCommandBuffer
{
    MZNT_RendererCommandBuffer parent;
    MZNT_VulkanRenderer*       renderer;
    VkCommandPool              cmdPool;
    VkCommandBuffer            cmdBuffer;
} MZNT_VulkanRendererCommandBuffer;

MZNT_VulkanRendererCommandBuffer* MZNT_CreateRendererCommandBuffer_Vulkan(MZNT_VulkanRenderer* renderer, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRendererCommandBuffer_Vulkan(MZNT_VulkanRendererCommandBuffer* commandBuffer, PNSLR_Allocator tempAllocator);

EXTERN_C_END
#endif
