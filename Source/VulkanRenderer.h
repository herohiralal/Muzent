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
PNSLR_DECLARE_ARRAY_SLICE(VkPresentModeKHR);

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

    VmaAllocator     vmaAllocator;
} MZNT_VulkanRenderer;

MZNT_VulkanRenderer* MZNT_CreateRenderer_Vulkan(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRenderer_Vulkan(MZNT_VulkanRenderer* renderer, PNSLR_Allocator tempAllocator);

typedef struct MZNT_VulkanRendererCommandBuffer
{
    MZNT_RendererCommandBuffer parent;
    MZNT_VulkanRenderer*       renderer;
    VkCommandPool              cmdPool;
    VkCommandBuffer            cmdBuffer;
} MZNT_VulkanRendererCommandBuffer;

typedef struct MZNT_VulkanRendererSurface
{
    MZNT_RendererSurface          parent;
    MZNT_VulkanRenderer*          renderer;
    VkSurfaceKHR                  surface;
    VkSwapchainKHR                swapchain;
    PNSLR_ArraySlice(VkImage)     swapchainImages;
    PNSLR_ArraySlice(VkImageView) swapchainImageViews;

    u32                              frameNumber;
    u32                              curSwpchImgIdx;
    MZNT_VulkanRendererCommandBuffer commandBuffers[MZNT_NUM_FRAMES_IN_FLIGHT];
    VkSemaphore                      swapchainSemaphore[MZNT_NUM_FRAMES_IN_FLIGHT];
    VkSemaphore                      renderSemaphore[MZNT_NUM_FRAMES_IN_FLIGHT];
    VkFence                          renderFence[MZNT_NUM_FRAMES_IN_FLIGHT];
} MZNT_VulkanRendererSurface;

MZNT_VulkanRendererSurface* MZNT_CreateRendererSurfaceFromWindow_Vulkan(MZNT_VulkanRenderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator);
b8 MZNT_ResizeRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator);

MZNT_VulkanRendererCommandBuffer* MZNT_BeginFrame_Vulkan(MZNT_VulkanRendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator);
b8 MZNT_EndFrame_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator);

EXTERN_C_END
#endif
