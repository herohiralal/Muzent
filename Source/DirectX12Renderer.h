#ifndef MZNT_DX12_PRIVATE_H
#define MZNT_DX12_PRIVATE_H
#include "__Prelude.h"
#include "Renderer.h"
EXTERN_C_BEGIN
#if defined(MZNT_IMPLEMENTATION) && MZNT_DX12

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

    void* memoryAllocator; // D3D12MA::Allocator*; can't make type safe here because it's a c++ class

    MZNT_DirectX12Shader triangleShader;
} MZNT_DirectX12Renderer;

MZNT_DirectX12Renderer* MZNT_CreateRenderer_DirectX12(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRenderer_DirectX12(MZNT_DirectX12Renderer* renderer, PNSLR_Allocator tempAllocator);

typedef struct MZNT_DirectX12RendererCommandBuffer
{
    MZNT_RendererCommandBuffer parent;
    MZNT_DirectX12Renderer*       renderer;
    // VkCommandBuffer            cmdBuffer;
} MZNT_DirectX12RendererCommandBuffer;

typedef struct MZNT_DirectX12RendererSurface
{
    MZNT_RendererSurface parent;

    MZNT_DirectX12Renderer* renderer; // owning renderer instance

    // main vk objs
    // VkSurfaceKHR  surface;
    // VkCommandPool cmdPool;

    // depth stuff
    // VkImage       depthImage;
    // VmaAllocation depthImageAllocation;
    // VkImageView   depthImageView;

    // shaders
    // VkPipeline trianglePipeline;

    // swapchain and related
    // VkSwapchainKHR                swapchain;
    // VkExtent2D                    swapchainExtent;
    // VkSurfaceFormatKHR            swapchainImageFormat;
    // PNSLR_ArraySlice(VkImage)     swapchainImages;
    // PNSLR_ArraySlice(VkImageView) swapchainImageViews;

    // synchronization
    u32                              curFrame;
    u32                              semIdx;
    u32                              curSwpchImgIdx;
    MZNT_DirectX12RendererCommandBuffer commandBuffers[MZNT_NUM_FRAMES_IN_FLIGHT];
    // PNSLR_ArraySlice(VkSemaphore)    presentCompleteSemaphores;
    // PNSLR_ArraySlice(VkSemaphore)    renderFinishedSemaphores;
    // PNSLR_ArraySlice(VkFence)        inFlightFences;
} MZNT_DirectX12RendererSurface;

MZNT_DirectX12RendererSurface* MZNT_CreateRendererSurfaceFromWindow_DirectX12(MZNT_DirectX12Renderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyRendererSurface_DirectX12(MZNT_DirectX12RendererSurface* surface, PNSLR_Allocator tempAllocator);
b8 MZNT_ResizeRendererSurface_DirectX12(MZNT_DirectX12RendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator);

MZNT_DirectX12RendererCommandBuffer* MZNT_BeginFrame_DirectX12(MZNT_DirectX12RendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator);
b8 MZNT_EndFrame_DirectX12(MZNT_DirectX12RendererSurface* surface, PNSLR_Allocator tempAllocator);

typedef struct MZNT_DirectX12Mesh
{
    MZNT_Mesh             parent;
    MZNT_DirectX12Renderer*  owner;

    // VkBuffer      meshBuffer;
    // VmaAllocation meshAllocation;

    MZNT_IndexType idxType;

    u32             vertexCount;
    u32             indexCount;
    i64             vertexBufferSize;
    i64             indexBufferSize;
} MZNT_DirectX12Mesh;

MZNT_DirectX12Mesh* MZNT_UploadMesh_DirectX12(MZNT_DirectX12Renderer* renderer, MZNT_MeshCreateInfo* createInfo, PNSLR_Allocator tempAllocator);
b8 MZNT_DestroyMesh_DirectX12(MZNT_DirectX12Mesh* mesh, PNSLR_Allocator tempAllocator);

#endif
EXTERN_C_END
#endif
