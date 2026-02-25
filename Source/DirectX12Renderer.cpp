#define MZNT_IMPLEMENTATION
#include "DirectX12Renderer.h"
#if MZNT_DX12

#define INLINED_FILE_INCLUSION_NAME k_MZNT_Internal_TriangleShaderVS
#include "Shaders/triangle_dxil_vs.c"
#undef INLINED_FILE_INCLUSION_NAME

#define INLINED_FILE_INCLUSION_NAME k_MZNT_Internal_TriangleShaderPS
#include "Shaders/triangle_dxil_ps.c"
#undef INLINED_FILE_INCLUSION_NAME

static inline D3D12MA::Allocator*& MZNT_Internal_GetAllocator(MZNT_DirectX12Renderer* renderer) { return (D3D12MA::Allocator*&) renderer->memoryAllocator; }

static inline void MZNT_Internal_LogDx12ResultOnFailure(HRESULT result, utf8str fnCall, PNSLR_SourceCodeLocation loc)
{
    if (SUCCEEDED(result)) return;
    utf8str message = {0};
    switch (result)
    {
        case D3D12_ERROR_ADAPTER_NOT_FOUND: message = Panshilar::StringLiteral("The specified cached PSO was created on a different adapter and cannot be reused on the current adapter."); break;
        case D3D12_ERROR_DRIVER_VERSION_MISMATCH: message = Panshilar::StringLiteral("The specified cached PSO was created on a different driver version and cannot be reused on the current adapter."); break;
        case DXGI_ERROR_INVALID_CALL: message = Panshilar::StringLiteral("The method call is invalid. For example, a method's parameter may not be a valid pointer."); break;
        case DXGI_ERROR_WAS_STILL_DRAWING: message = Panshilar::StringLiteral("The previous blit operation that is transferring information to or from this surface is incomplete."); break;
        case E_FAIL: message = Panshilar::StringLiteral("Attempted to create a device with the debug layer enabled and the layer is not installed."); break;
        case E_INVALIDARG: message = Panshilar::StringLiteral("An invalid parameter was passed to the returning function."); break;
        case E_OUTOFMEMORY: message = Panshilar::StringLiteral("Direct3D could not allocate sufficient memory to complete the call."); break;
        case E_NOTIMPL: message = Panshilar::StringLiteral("The method call isn't implemented with the passed parameter combination."); break;
        case S_FALSE: message = Panshilar::StringLiteral("Alternate success value, indicating a successful but nonstandard completion (the precise meaning depends on context)."); break;
        default: message = Panshilar::StringLiteral("An unknown error occurred."); break;
    }

    if (result != S_OK)
    {
        PNSLR_LogEf(Panshilar::StringLiteral("DirectX 12 error: $ from $"),
                    PNSLR_FmtArgs(
                        PNSLR_FmtString(message),
                        PNSLR_FmtString(fnCall)
                    ),
                    loc);

        FORCE_DBG_TRAP;
    }
}

#define MZNT_INTERNAL_DX12_CHECKED_CALL(call) \
    MZNT_Internal_LogDx12ResultOnFailure((call), Panshilar::StringLiteral(#call), PNSLR_GET_LOC())

static const DXGI_FORMAT k_MZNT_Internal_PreferredColourAttchFormat  = DXGI_FORMAT_R16G16B16A16_FLOAT;
static const DXGI_FORMAT k_MZNT_Internal_PreferredDepthAttchFormat   = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

MZNT_DirectX12Renderer* MZNT_CreateRenderer_DirectX12(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator)
{
    MZNT_DirectX12Renderer* output = PNSLR_New(MZNT_DirectX12Renderer, config.allocator, PNSLR_GET_LOC(), nil);
    if (!output) FORCE_DBG_TRAP;

    output->parent.type      = MZNT_RendererType_DirectX12;
    output->parent.allocator = config.allocator;
    output->parent.appHandle = config.appHandle;

    // debug layer
    u32 dxgiFactoryFlags = 0;
    if (PNSLR_DBG)
    {
        MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12GetDebugInterface(IID_PPV_ARGS(&(output->dbgController))));
        output->dbgController->EnableDebugLayer();

        ID3D12Debug1* dbgController1;
        if (SUCCEEDED(output->dbgController->QueryInterface(IID_PPV_ARGS(&dbgController1))))
        {
            dbgController1->SetEnableGPUBasedValidation(true);
            dbgController1->Release();
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }

    // factory
    MZNT_INTERNAL_DX12_CHECKED_CALL(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&(output->dxgiFactory))));

    // adapter
    {
        IDXGIAdapter1* hwAdapter;
        for (u32 i = 0; output->dxgiFactory->EnumAdapters1(i, &hwAdapter) != DXGI_ERROR_NOT_FOUND; i++)
        {
            DXGI_ADAPTER_DESC1 desc;
            hwAdapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                hwAdapter->Release();
                continue;
            }

            if (SUCCEEDED(D3D12CreateDevice(hwAdapter, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), nil)))
            {
                output->adapter = hwAdapter;
                break;
            }
        }

        if (!output->adapter)
        {
            PNSLR_LogE(Panshilar::StringLiteral("No compatible DirectX 12 adapter found."), PNSLR_GET_LOC());
            FORCE_DBG_TRAP;
            return nil;
        }
    }

    // device
    MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12CreateDevice(output->adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&(output->device))));

    // queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = { };
    queueDesc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    MZNT_INTERNAL_DX12_CHECKED_CALL(output->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&(output->cmdQueue))));

    // allocator
    D3D12MA::ALLOCATOR_DESC allocDesc = { };
    allocDesc.pDevice = output->device;
    allocDesc.pAdapter = output->adapter;
    MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12MA::CreateAllocator(&allocDesc, &MZNT_Internal_GetAllocator(output)));

    // triangle shader
    {
        // root signature
        {
            D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc = {};
            rootDesc.Version                    = D3D_ROOT_SIGNATURE_VERSION_1_0;
            rootDesc.Desc_1_0.NumParameters     = 0;
            rootDesc.Desc_1_0.pParameters       = nil;
            rootDesc.Desc_1_0.NumStaticSamplers = 0;
            rootDesc.Desc_1_0.pStaticSamplers   = nil;
            rootDesc.Desc_1_0.Flags             = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

            ID3DBlob* serializedDesc = nil;
            MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12SerializeVersionedRootSignature(&rootDesc, &serializedDesc, nil));
            MZNT_INTERNAL_DX12_CHECKED_CALL(output->device->CreateRootSignature(0, serializedDesc->GetBufferPointer(), serializedDesc->GetBufferSize(), IID_PPV_ARGS(&(output->triangleShader.rootSignature))));
            serializedDesc->Release();
        }

        // pso
        {
            D3D12_INPUT_LAYOUT_DESC inputLayout = {};
            inputLayout.pInputElementDescs = nil;
            inputLayout.NumElements        = 0;

            D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { };
            psoDesc.pRootSignature                  = output->triangleShader.rootSignature;
            psoDesc.VS.pShaderBytecode              = k_MZNT_Internal_TriangleShaderVSContents;
            psoDesc.VS.BytecodeLength               = k_MZNT_Internal_TriangleShaderVSSize;
            psoDesc.PS.pShaderBytecode              = k_MZNT_Internal_TriangleShaderPSContents;
            psoDesc.PS.BytecodeLength               = k_MZNT_Internal_TriangleShaderPSSize;
            psoDesc.InputLayout.pInputElementDescs  = inputLayout.pInputElementDescs;
            psoDesc.InputLayout.NumElements         = inputLayout.NumElements;
            psoDesc.PrimitiveTopologyType           = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            psoDesc.RTVFormats[0]                   = DXGI_FORMAT_B8G8R8A8_UNORM;
            psoDesc.NumRenderTargets                = 1;
            psoDesc.SampleDesc.Count                = 1;
            psoDesc.SampleMask                      = UINT_MAX;
            psoDesc.RasterizerState                 = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
            psoDesc.RasterizerState.FrontCounterClockwise = true;
            psoDesc.BlendState                      = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
            psoDesc.DepthStencilState.DepthEnable   = false;
            psoDesc.DepthStencilState.StencilEnable = false;

            MZNT_INTERNAL_DX12_CHECKED_CALL(output->device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&(output->triangleShader.pipelineState))));
        }
    }

    return output;
}

b8 MZNT_DestroyRenderer_DirectX12(MZNT_DirectX12Renderer* renderer, PNSLR_Allocator tempAllocator)
{
    if (!renderer) return false;

    renderer->triangleShader.pipelineState->Release();
    renderer->triangleShader.rootSignature->Release();

    MZNT_Internal_GetAllocator(renderer)->Release();

    renderer->cmdQueue->Release();

    renderer->device->Release();

    renderer->adapter->Release();

    renderer->dxgiFactory->Release();

    if (renderer->dbgController)
    {
        ID3D12Debug1* dbgController1;
        if (SUCCEEDED(renderer->dbgController->QueryInterface(IID_PPV_ARGS(&dbgController1))))
        {
            dbgController1->SetEnableGPUBasedValidation(false);
            dbgController1->Release();
        }

        ID3D12Debug4* dbgController4;
        if (SUCCEEDED(renderer->dbgController->QueryInterface(IID_PPV_ARGS(&dbgController4))))
        {
            dbgController4->DisableDebugLayer();
            dbgController4->Release();
        }

        renderer->dbgController->Release();
    }

    PNSLR_Delete(renderer, renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    return true;
}

static void MZNT_Internal_WaitForDx12GPUToBeIdle(MZNT_DirectX12RendererSurface* surface)
{
    UINT64 fenceValue = ++surface->nextFenceValue;
    MZNT_INTERNAL_DX12_CHECKED_CALL(surface->renderer->cmdQueue->Signal(surface->fence, fenceValue));

    if (surface->fence->GetCompletedValue() < fenceValue)
    {
        MZNT_INTERNAL_DX12_CHECKED_CALL(surface->fence->SetEventOnCompletion(fenceValue, surface->fenceEvent));
        WaitForSingleObject(surface->fenceEvent, INFINITE);
    }
}

MZNT_DirectX12RendererSurface* MZNT_CreateRendererSurfaceFromWindow_DirectX12(MZNT_DirectX12Renderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator)
{
    MZNT_DirectX12RendererSurface* output = PNSLR_New(MZNT_DirectX12RendererSurface, renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    if (!output) FORCE_DBG_TRAP;

    output->parent.type = MZNT_RendererType_DirectX12;
    output->renderer    = renderer;

    // swapchain & its properties
    {
        output->swapchainFormat = DXGI_FORMAT_B8G8R8A8_UNORM; // this is the most widely supported swapchain format, even though we render to a different format internally

        DXGI_SWAP_CHAIN_DESC1 swapchainDesc = { };
        swapchainDesc.Width            = 0; // use window's client area width
        swapchainDesc.Height           = 0; // use window's client area height
        swapchainDesc.Format           = output->swapchainFormat;
        swapchainDesc.Stereo           = FALSE;
        swapchainDesc.SampleDesc.Count = 1;
        swapchainDesc.BufferUsage      = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchainDesc.BufferCount      = MZNT_NUM_FRAMES_IN_FLIGHT;
        swapchainDesc.Scaling          = DXGI_SCALING_NONE;
        swapchainDesc.SwapEffect       = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapchainDesc.AlphaMode        = DXGI_ALPHA_MODE_UNSPECIFIED;

        HWND wnd;
        #if PNSLR_WINDOWS
        {
            wnd = (HWND) (uintptr_t) windowHandle.handle;
        }
        #else
            #error "Unsupported platform"
        #endif

        MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->dxgiFactory->CreateSwapChainForHwnd(renderer->cmdQueue, wnd, &swapchainDesc, nil, nil, (IDXGISwapChain1**) &(output->swapchain)));

        MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->dxgiFactory->MakeWindowAssociation(wnd, DXGI_MWA_NO_ALT_ENTER)); // disable automatic fullscreen transitions on alt+enter

        output->swapchain->GetDesc1(&swapchainDesc);
        output->swapchainWidth  = swapchainDesc.Width;
        output->swapchainHeight = swapchainDesc.Height;
    }

    // descriptor heap & its size
    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = { };
        heapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heapDesc.NumDescriptors = MZNT_NUM_FRAMES_IN_FLIGHT;
        heapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&(output->rtvHeap))));

        output->rtvDescriptorSize = renderer->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    // render target views
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = output->rtvHeap->GetCPUDescriptorHandleForHeapStart();
        for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        {
            ID3D12Resource* backBuffer;
            MZNT_INTERNAL_DX12_CHECKED_CALL(output->swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));
            renderer->device->CreateRenderTargetView(backBuffer, nil, rtvHandle);
            rtvHandle.ptr += output->rtvDescriptorSize;

            output->renderTargets[i] = backBuffer;
        }
    }

    // depth stencil buffer
    {
        D3D12_RESOURCE_DESC depthDesc = { };
        depthDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depthDesc.Width            = output->swapchainWidth;
        depthDesc.Height           = output->swapchainHeight;
        depthDesc.DepthOrArraySize = 1;
        depthDesc.MipLevels        = 1;
        depthDesc.Format           = k_MZNT_Internal_PreferredDepthAttchFormat;
        depthDesc.SampleDesc.Count = 1;
        depthDesc.Flags            = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        D3D12_CLEAR_VALUE depthOptimizedClearValue = { };
        depthOptimizedClearValue.Format             = k_MZNT_Internal_PreferredDepthAttchFormat;
        depthOptimizedClearValue.DepthStencil.Depth = 1.0f;

        D3D12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &depthDesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &depthOptimizedClearValue,
            IID_PPV_ARGS(&(output->depthBuffer))
        ));
    }

    // dsv heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = { };
        dsvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&(output->dsvHeap))));
        renderer->device->CreateDepthStencilView(output->depthBuffer, nil, output->dsvHeap->GetCPUDescriptorHandleForHeapStart());
    }

    // command allocators & command lists
    for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
    {
        MZNT_DirectX12RendererCommandBuffer& cmdBuffer = output->commandBuffers[i];
        MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&(cmdBuffer.cmdAllocator))));
        MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdBuffer.cmdAllocator, nil, IID_PPV_ARGS(&(cmdBuffer.cmdList))));

        // it starts in the recording state...
        cmdBuffer.cmdList->Close();
    }

    // fence, fence value, fence event
    output->nextFenceValue = 0;
    for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        output->frameFenceValues[i] = 0;
    MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&(output->fence))));
    output->fenceEvent = CreateEventA(nil, FALSE, FALSE, nil);
    if (!output->fenceEvent) FORCE_DBG_TRAP;

    // frame idx
    output->curFrame = output->swapchain->GetCurrentBackBufferIndex();

    return output;
}

b8 MZNT_DestroyRendererSurface_DirectX12(MZNT_DirectX12RendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    if (!surface) return false;

    MZNT_Internal_WaitForDx12GPUToBeIdle(surface);

    // destroy command allocators & command lists
    for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
    {
        MZNT_DirectX12RendererCommandBuffer& cmdBuffer = surface->commandBuffers[i];
        cmdBuffer.cmdList->Release();
        cmdBuffer.cmdAllocator->Release();
    }

    // fences and events
    CloseHandle(surface->fenceEvent);
    surface->fence->Release();

    // depth stencil buffer and its view
    surface->dsvHeap->Release();
    surface->depthBuffer->Release();

    // render target views and swapchain buffers
    for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        surface->renderTargets[i]->Release();

    // rtv heap
    surface->rtvHeap->Release();

    // swapchain
    surface->swapchain->Release();

    PNSLR_Delete(surface, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    return true;
}

b8 MZNT_ResizeRendererSurface_DirectX12(MZNT_DirectX12RendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator)
{
    // TODO: resize depth image as well
    if (!surface) return false;
    if (!surface->renderer) FORCE_DBG_TRAP;

    MZNT_Internal_WaitForDx12GPUToBeIdle(surface);

    // release render target views
    for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        surface->renderTargets[i]->Release();

    // resize buffers
    MZNT_INTERNAL_DX12_CHECKED_CALL(surface->swapchain->ResizeBuffers(MZNT_NUM_FRAMES_IN_FLIGHT, width, height, surface->swapchainFormat, 0));

    // update height/width
    {
        DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
        surface->swapchain->GetDesc1(&swapchainDesc);
        surface->swapchainWidth  = swapchainDesc.Width;
        surface->swapchainHeight = swapchainDesc.Height;
    }

    // render target views
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = surface->rtvHeap->GetCPUDescriptorHandleForHeapStart();
        for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        {
            ID3D12Resource* backBuffer;
            MZNT_INTERNAL_DX12_CHECKED_CALL(surface->swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));
            surface->renderer->device->CreateRenderTargetView(backBuffer, nil, rtvHandle);
            rtvHandle.ptr += surface->rtvDescriptorSize;

            surface->renderTargets[i] = backBuffer;
        }
    }

    return true;
}

MZNT_DirectX12RendererCommandBuffer* MZNT_BeginFrame_DirectX12(MZNT_DirectX12RendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator)
{
    if (!surface) return nil;
    if (!surface->renderer) FORCE_DBG_TRAP;

    surface->curFrame = surface->swapchain->GetCurrentBackBufferIndex();

    if (surface->fence->GetCompletedValue() < surface->frameFenceValues[surface->curFrame])
    {
        MZNT_INTERNAL_DX12_CHECKED_CALL(surface->fence->SetEventOnCompletion(surface->frameFenceValues[surface->curFrame], surface->fenceEvent));
        WaitForSingleObject(surface->fenceEvent, INFINITE);
    }

    MZNT_DirectX12RendererCommandBuffer& cmdBuffer = surface->commandBuffers[surface->curFrame];
    cmdBuffer.cmdAllocator->Reset();
    cmdBuffer.cmdList->Reset(cmdBuffer.cmdAllocator, nil);

    // transition present -> rt
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            surface->renderTargets[surface->curFrame],
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET);

        cmdBuffer.cmdList->ResourceBarrier(1, &barrier);
    }

    // bind rt
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtv = surface->rtvHeap->GetCPUDescriptorHandleForHeapStart();
        rtv.ptr += surface->curFrame * surface->rtvDescriptorSize;

        cmdBuffer.cmdList->OMSetRenderTargets(1, &rtv, FALSE, nil);

        float clearColor[4] = {r, g, b, a};
        cmdBuffer.cmdList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
    }

    // viewport & scissor
    {
        D3D12_VIEWPORT vp = { };
        vp.Width  = (float) surface->swapchainWidth;
        vp.Height = (float) surface->swapchainHeight;
        vp.MaxDepth = 1.0f;
        cmdBuffer.cmdList->RSSetViewports(1, &vp);

        D3D12_RECT scissor = { };
        scissor.right  = (LONG) surface->swapchainWidth;
        scissor.bottom = (LONG) surface->swapchainHeight;
        cmdBuffer.cmdList->RSSetScissorRects(1, &scissor);
    }

    cmdBuffer.cmdList->SetPipelineState(surface->renderer->triangleShader.pipelineState);
    cmdBuffer.cmdList->SetGraphicsRootSignature(surface->renderer->triangleShader.rootSignature);
    cmdBuffer.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdBuffer.cmdList->DrawInstanced(3, 1, 0, 0);

    return &cmdBuffer;
}

b8 MZNT_EndFrame_DirectX12(MZNT_DirectX12RendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    if (!surface) return false;
    if (!surface->renderer) FORCE_DBG_TRAP;

    MZNT_DirectX12RendererCommandBuffer& cmdBuffer = surface->commandBuffers[surface->curFrame];

    // transition rt -> present
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            surface->renderTargets[surface->curFrame],
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT);

        cmdBuffer.cmdList->ResourceBarrier(1, &barrier);
    }

    cmdBuffer.cmdList->Close();

    // submit
    ID3D12CommandList* cmdLists[] = { cmdBuffer.cmdList };
    surface->renderer->cmdQueue->ExecuteCommandLists(1, cmdLists);

    // present
    MZNT_INTERNAL_DX12_CHECKED_CALL(surface->swapchain->Present(1, 0));

    surface->nextFenceValue++;
    surface->frameFenceValues[surface->curFrame] = surface->nextFenceValue;
    MZNT_INTERNAL_DX12_CHECKED_CALL(surface->renderer->cmdQueue->Signal(surface->fence, surface->nextFenceValue));
    return true;
}

MZNT_DirectX12Mesh* MZNT_UploadMesh_DirectX12(MZNT_DirectX12Renderer* renderer, MZNT_MeshCreateInfo* createInfo, PNSLR_Allocator tempAllocator)
{
    return nil;
}

b8 MZNT_DestroyMesh_DirectX12(MZNT_DirectX12Mesh* mesh, PNSLR_Allocator tempAllocator)
{
    return false;
}

#endif
