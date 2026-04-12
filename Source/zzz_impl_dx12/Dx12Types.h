#ifndef MZNT_DX12_TYPES_H
#define MZNT_DX12_TYPES_H
#include "../__Prelude.h"
#include "../Renderer.h"
#if defined(MZNT_IMPLEMENTATION) && MZNT_DX12
EXTERN_C_BEGIN

// d3d12ma struct ptr fwds
#ifdef __cplusplus
typedef class D3D12MA::Allocator* DxAllocator;
typedef class D3D12MA::Allocation* DxAllocation;
#else
typedef struct DxAllocator* DxAllocator;
typedef struct DxAllocation* DxAllocation;
#endif

// shader
typedef struct MZNT_DirectX12Shader
{
    ID3D12RootSignature* rootSignature;
    ID3D12PipelineState* pipelineState;
} MZNT_DirectX12Shader;

// renderer
typedef struct MZNT_DirectX12Renderer
{
    MZNT_Renderer       parent;
    IDXGIFactory6*      dxgiFactory;
    IDXGIAdapter1*      adapter;
    ID3D12Device2*      device;
    ID3D12CommandQueue* cmdQueue;

    ID3D12Debug* dbgController;
    u32          dbgCallbackCookie;

    DxAllocator d3d12maAllocator;

    MZNT_DirectX12Shader helloTriangleVertexShadedProgram;
    MZNT_DirectX12Shader helloTriangleMeshShadedProgram;
} MZNT_DirectX12Renderer;

// cmd buffer
typedef struct MZNT_DirectX12RendererCommandBuffer
{
    MZNT_RendererCommandBuffer  parent;
    MZNT_DirectX12Renderer*     renderer;
    ID3D12CommandAllocator*     cmdAllocator;
    ID3D12GraphicsCommandList6* cmdList;
} MZNT_DirectX12RendererCommandBuffer;

// surface
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

// swap chain
typedef struct MZNT_DirectX12SwapChain
{
    MZNT_SwapChain parent;
} MZNT_DirectX12SwapChain;

EXTERN_C_END
#endif
#endif
