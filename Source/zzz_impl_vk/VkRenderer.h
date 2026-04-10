#ifndef MZNT_VULKAN_PRIVATE_H
#define MZNT_VULKAN_PRIVATE_H
#include "../__Prelude.h"
#include "../Renderer.h"
EXTERN_C_BEGIN
#if defined(MZNT_IMPLEMENTATION) && MZNT_VULKAN

PNSLR_DECLARE_ARRAY_SLICE(VkLayerProperties);
PNSLR_DECLARE_ARRAY_SLICE(VkExtensionProperties);
PNSLR_DECLARE_ARRAY_SLICE(VkPhysicalDevice);
PNSLR_DECLARE_ARRAY_SLICE(VkQueueFamilyProperties);
PNSLR_DECLARE_ARRAY_SLICE(VkDeviceQueueCreateInfo);
PNSLR_DECLARE_ARRAY_SLICE(VkSurfaceFormatKHR);
PNSLR_DECLARE_ARRAY_SLICE(VkImage);
PNSLR_DECLARE_ARRAY_SLICE(VkImageView);
PNSLR_DECLARE_ARRAY_SLICE(VkFramebuffer);
PNSLR_DECLARE_ARRAY_SLICE(VkPresentModeKHR);
PNSLR_DECLARE_ARRAY_SLICE(VkFence);
PNSLR_DECLARE_ARRAY_SLICE(VkSemaphore);

PNSLR_DECLARE_ARRAY_SLICE(VmaAllocation);

typedef struct MZNT_VulkanShader
{
    VkShaderModule        vsModule;
    VkShaderModule        msModule;
    VkShaderModule        fsModule;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout      pipelineLayout;
    VkPipeline            pipeline;
} MZNT_VulkanShader;

typedef struct MZNT_VulkanRenderer
{
    MZNT_Renderer    parent;
    VkInstance       instance;
    VkPhysicalDevice physicalDevice;
    VkDevice         device;
    u32              gfxQueueFamilyIndex;
    u32              presQueueFamilyIndex;
    VkQueue          gfxQueue;
    VkQueue          presQueue;

    VkDebugUtilsMessengerEXT debugMessenger;

    VmaAllocator     vmaAllocator;

    MZNT_VulkanShader helloTriangleVertexShadedProgram;
    MZNT_VulkanShader helloTriangleMeshShadedProgram;
} MZNT_VulkanRenderer;

MZNT_VulkanRenderer* MZNT_CreateRenderer_Vulkan(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRenderer_Vulkan(MZNT_VulkanRenderer* renderer, PNSLR_Allocator tempAllocator);

typedef struct MZNT_VulkanRendererCommandBuffer
{
    MZNT_RendererCommandBuffer parent;
    MZNT_VulkanRenderer*       renderer;
    VkCommandBuffer            cmdBuffer;
} MZNT_VulkanRendererCommandBuffer;

typedef struct MZNT_VulkanRendererSurface
{
    MZNT_RendererSurface parent;

    MZNT_VulkanRenderer* renderer; // owning renderer instance

    // main vk objs
    VkSurfaceKHR  surface;
    VkCommandPool cmdPool;

    // screen buffer
    PNSLR_ArraySlice(VkImage)       screenImages;
    PNSLR_ArraySlice(VmaAllocation) screenImageAllocations;
    PNSLR_ArraySlice(VkImageView)   screenImageViews;

    // depth stuff
    PNSLR_ArraySlice(VkImage)       depthImages;
    PNSLR_ArraySlice(VmaAllocation) depthImageAllocations;
    PNSLR_ArraySlice(VkImageView)   depthImageViews;

    // swapchain and related
    VkSwapchainKHR                swapchain;
    VkExtent2D                    swapchainExtent;
    VkSurfaceFormatKHR            swapchainImageFormat;
    PNSLR_ArraySlice(VkImage)     swapchainImages;
    PNSLR_ArraySlice(VkImageView) swapchainImageViews;

    // synchronization
    u32                              curFrame;
    u32                              semIdx;
    u32                              curSwpchImgIdx;
    MZNT_VulkanRendererCommandBuffer commandBuffers[MZNT_NUM_FRAMES_IN_FLIGHT];
    PNSLR_ArraySlice(VkSemaphore)    presentCompleteSemaphores;
    PNSLR_ArraySlice(VkSemaphore)    renderFinishedSemaphores;
    PNSLR_ArraySlice(VkFence)        inFlightFences;

    // shaders
    MZNT_VulkanShader finalBlitShader;
    VkSampler         finalBlitSampler;
    VkDescriptorPool  finalBlitDescriptorPool;
    VkDescriptorSet   finalBlitDescriptorSets[MZNT_NUM_FRAMES_IN_FLIGHT];
} MZNT_VulkanRendererSurface;

MZNT_VulkanRendererSurface* MZNT_CreateRendererSurfaceFromWindow_Vulkan(MZNT_VulkanRenderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator);
b8 MZNT_ResizeRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator);

MZNT_VulkanRendererCommandBuffer* MZNT_BeginFrame_Vulkan(MZNT_VulkanRendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator);
b8 MZNT_EndFrame_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator);

#endif
EXTERN_C_END
#endif
