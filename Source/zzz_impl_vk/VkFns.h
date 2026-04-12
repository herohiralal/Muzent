#ifndef MZNT_VK_FNS_H
#define MZNT_VK_FNS_H
#include "../__Prelude.h"
#include "../Renderer.h"
#include "VkTypes.h"
EXTERN_C_BEGIN
#if defined(MZNT_IMPLEMENTATION) && MZNT_VULKAN

inline PFN_vkDebugUtilsMessengerCallbackEXT MZNT_Internal_GetVkDebugCallback(void);
void MZNT_Internal_LogVkResultOnFailure(VkResult result, utf8str fnCall, PNSLR_SourceCodeLocation loc);

#define MZNT_INTERNAL_VK_CHECKED_CALL(call) \
    MZNT_Internal_LogVkResultOnFailure((call), PNSLR_StringLiteral(#call), PNSLR_GET_LOC())

inline VkFormat MZNT_Internal_BreakVkTextureFormat(MZNT_TextureFormat fmt);
inline MZNT_TextureFormat MZNT_Internal_MakeVkTextureFormat(VkFormat fmt);

MZNT_VulkanRenderer* MZNT_CreateRenderer_Vulkan(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator);
b8 MZNT_WaitTillRendererIdle_Vulkan(const MZNT_VulkanRenderer* renderer);
b8 MZNT_DestroyRenderer_Vulkan(MZNT_VulkanRenderer* renderer, PNSLR_Allocator tempAllocator);

MZNT_VulkanSwapChain* MZNT_CreateSwapChainFromWindow_Vulkan(MZNT_VulkanRenderer* renderer, MZNT_WindowHandle windowHandle, MZNT_SwapChainConfiguration cfg, PNSLR_Allocator tempAllocator);
b8 MZNT_ReconfigureSwapChain_Vulkan(MZNT_VulkanSwapChain* swapChain, MZNT_SwapChainConfiguration cfg, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroySwapChain_Vulkan(MZNT_VulkanSwapChain* swapChain, PNSLR_Allocator tempAllocator);
MZNT_TextureFormat MZNT_GetSwapChainTextureFormat_Vulkan(const MZNT_VulkanSwapChain* swapChain);
MZNT_VulkanRendererCommandBuffer* MZNT_IterateSwapChain_Vulkan(MZNT_VulkanSwapChain* swapChain, u8* outImgIdx, PNSLR_Allocator tempAllocator);
b8 MZNT_PresentSwapChain_Vulkan(const MZNT_VulkanSwapChain* swapChain, PNSLR_Allocator tempAllocator);

MZNT_VulkanRendererSurface* MZNT_CreateRendererSurfaceFromWindow_Vulkan(MZNT_VulkanRenderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator);
b8 MZNT_ResizeRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator);

MZNT_VulkanRendererCommandBuffer* MZNT_BeginFrame_Vulkan(MZNT_VulkanRendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator);
b8 MZNT_EndFrame_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator);

#endif
EXTERN_C_END
#endif
