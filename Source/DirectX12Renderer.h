#ifndef MZNT_DX12_PRIVATE_H
#define MZNT_DX12_PRIVATE_H
#include "__Prelude.h"
#include "Renderer.h"
EXTERN_C_BEGIN
#if defined(MZNT_IMPLEMENTATION) && MZNT_DX12

#ifdef __cplusplus
typedef class D3D12MA::Allocator* DxAllocator;
typedef class D3D12MA::Allocation* DxAllocation;
#else
typedef struct DxAllocator* DxAllocator;
typedef struct DxAllocation* DxAllocation;
#endif

typedef struct MZNT_DirectX12Shader
{
    ID3D12RootSignature* rootSignature;
    ID3D12PipelineState* pipelineState;
} MZNT_DirectX12Shader;

typedef struct MZNT_DirectX12Renderer
{
    MZNT_Renderer       parent;
    IDXGIFactory6*      dxgiFactory;
    IDXGIAdapter1*      adapter;
    ID3D12Device*       device;
    ID3D12CommandQueue* cmdQueue;

    ID3D12Debug* dbgController;
    u32          dbgCallbackCookie;

    DxAllocator d3d12maAllocator;

    MZNT_DirectX12Shader helloTriangleShader;
} MZNT_DirectX12Renderer;

MZNT_DirectX12Renderer* MZNT_CreateRenderer_DirectX12(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRenderer_DirectX12(MZNT_DirectX12Renderer* renderer, PNSLR_Allocator tempAllocator);

typedef struct MZNT_DirectX12RendererCommandBuffer
{
    MZNT_RendererCommandBuffer parent;
    MZNT_DirectX12Renderer*    renderer;
    ID3D12CommandAllocator*    cmdAllocator;
    ID3D12GraphicsCommandList* cmdList;
} MZNT_DirectX12RendererCommandBuffer;

typedef struct MZNT_DirectX12RendererSurface
{
    MZNT_RendererSurface    parent;
    MZNT_DirectX12Renderer* renderer; // owning renderer instance

    // swapchain
    IDXGISwapChain4*      swapchain;
    UINT                  swapchainWidth, swapchainHeight;
    DXGI_FORMAT           swapchainFormat;
    ID3D12DescriptorHeap* swapchainRtvHeap;
    UINT                  swapchainRtvDescriptorSize;
    ID3D12Resource*       swapchainRTs[MZNT_NUM_FRAMES_IN_FLIGHT]; // back buffers from the swapchain

    // screen buffer
    ID3D12Resource*       screenBuffer[MZNT_NUM_FRAMES_IN_FLIGHT];
    DxAllocation          screenBufferAllocations[MZNT_NUM_FRAMES_IN_FLIGHT];
    ID3D12DescriptorHeap* svRtvHeap;
    UINT                  svRtvDescriptorSize;
    ID3D12DescriptorHeap* svSrvHeap;
    UINT                  svSrvDescriptorSize;

    // depth
    ID3D12Resource*       depthBuffer[MZNT_NUM_FRAMES_IN_FLIGHT];
    DxAllocation          depthBufferAllocations[MZNT_NUM_FRAMES_IN_FLIGHT];
    ID3D12DescriptorHeap* dsvHeap;
    UINT                  dsvDescriptorSize;

    // command buffers (frames in flight)
    MZNT_DirectX12RendererCommandBuffer commandBuffers[MZNT_NUM_FRAMES_IN_FLIGHT];

    // synchronization
    UINT         curFrame;
    ID3D12Fence* fence;
    UINT64       frameFenceValues[MZNT_NUM_FRAMES_IN_FLIGHT];
    UINT64       nextFenceValue;
    HANDLE       fenceEvent;

    // shaders
    MZNT_DirectX12Shader finalBlitShader;
} MZNT_DirectX12RendererSurface;

MZNT_DirectX12RendererSurface* MZNT_CreateRendererSurfaceFromWindow_DirectX12(MZNT_DirectX12Renderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRendererSurface_DirectX12(MZNT_DirectX12RendererSurface* surface, PNSLR_Allocator tempAllocator);
b8 MZNT_ResizeRendererSurface_DirectX12(MZNT_DirectX12RendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator);

MZNT_DirectX12RendererCommandBuffer* MZNT_BeginFrame_DirectX12(MZNT_DirectX12RendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator);
b8 MZNT_EndFrame_DirectX12(MZNT_DirectX12RendererSurface* surface, PNSLR_Allocator tempAllocator);

#endif
EXTERN_C_END
#endif
