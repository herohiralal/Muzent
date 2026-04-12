#ifndef MZNT_VK_TYPES_H
#define MZNT_VK_TYPES_H
#include "../__Prelude.h"
#include "../Renderer.h"
#if defined(MZNT_IMPLEMENTATION) && MZNT_VULKAN
EXTERN_C_BEGIN

// vulkan array slices
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

// vma array slices
PNSLR_DECLARE_ARRAY_SLICE(VmaAllocation);

// shader
typedef struct MZNT_VulkanShader
{
    VkShaderModule        vsModule;
    VkShaderModule        msModule;
    VkShaderModule        fsModule;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout      pipelineLayout;
    VkPipeline            pipeline;
} MZNT_VulkanShader;

// renderer
typedef struct alignas(64) MZNT_VulkanRenderer
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

// cmd buffer
typedef struct MZNT_VulkanRendererCommandBuffer
{
    MZNT_RendererCommandBuffer parent;
    MZNT_VulkanRenderer*       renderer;
    VkCommandPool              cmdPool;
    VkCommandBuffer            cmdBuffer;
} MZNT_VulkanRendererCommandBuffer;

PNSLR_DECLARE_ARRAY_SLICE(MZNT_VulkanRendererCommandBuffer);

// swap chain
typedef struct alignas(64) MZNT_VulkanSwapChain
{
    MZNT_SwapChain       parent;
    MZNT_VulkanRenderer* renderer;
    VkSwapchainKHR       actual;

    // cfg
    b8 vSync;
    u8 framesInFlight;

    // surface info
    VkSurfaceKHR         surface;
    VkSurfaceFormatKHR   surfaceFmt;
    VkExtent2D           surfaceSize;

    // syncing
    b8                            noCmdBufThisFrame;
    u32                           curFrame, semIdx, curImgIdx;
    PNSLR_ArraySlice(VkSemaphore) presentCompleteSemaphores;
    PNSLR_ArraySlice(VkSemaphore) renderFinishedSemaphores;
    PNSLR_ArraySlice(VkFence)     inFlightFences;

    // images
    PNSLR_ArraySlice(VkImage)     imgs;
    PNSLR_ArraySlice(VkImageView) imgViews;

    // command buffers
    PNSLR_ArraySlice(MZNT_VulkanRendererCommandBuffer) cmdBuffers;
} MZNT_VulkanSwapChain;

// surface
typedef struct MZNT_VulkanRendererSurface
{
    MZNT_RendererSurface parent;

    MZNT_VulkanRenderer* renderer; // owning renderer instance

    // main vk objs
    VkSurfaceKHR  surface;

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

EXTERN_C_END
#endif
#endif
