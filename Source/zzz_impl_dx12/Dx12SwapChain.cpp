#define MZNT_IMPLEMENTATION
#include "Dx12Fns.h"
#if MZNT_DX12

MZNT_DirectX12SwapChain* MZNT_CreateSwapChainFromWindow_DirectX12(MZNT_DirectX12Renderer* renderer, MZNT_WindowHandle windowHandle, MZNT_SwapChainConfiguration cfg, PNSLR_Allocator tempAllocator)
{
    return nullptr;
}

b8 MZNT_ReconfigureSwapChain_DirectX12(MZNT_DirectX12SwapChain* swapChain, MZNT_SwapChainConfiguration cfg, PNSLR_Allocator tempAllocator)
{
    return false;
}

b8 MZNT_DestroySwapChain_DirectX12(MZNT_DirectX12SwapChain* swapChain, PNSLR_Allocator tempAllocator)
{
    return false;
}

MZNT_TextureFormat MZNT_GetSwapChainTextureFormat_DirectX12(const MZNT_DirectX12SwapChain* swapChain)
{
    return MZNT_TextureFormat_Unknown;
}

b8 MZNT_IterateSwapChain_DirectX12(MZNT_DirectX12SwapChain* swapChain, PNSLR_Allocator tempAllocator)
{
    return false;
}

MZNT_DirectX12RendererCommandBuffer* MZNT_GetSwapChainCommandBuffer_DirectX12(const MZNT_DirectX12SwapChain* swapChain, u8* outImgIdx, PNSLR_Allocator tempAllocator)
{
    return nullptr;
}

b8 MZNT_PresentSwapChain_DirectX12(const MZNT_DirectX12SwapChain* swapChain, PNSLR_Allocator tempAllocator)
{
    return false;
}

#endif
