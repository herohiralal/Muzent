#define MZNT_IMPLEMENTATION
#include "Dx12Fns.h"
#if MZNT_DX12

#define INLINED_FILE_INCLUSION_NAME k_MZNT_Internal_Dx12HelloTriangleVS
#include "../Shaders/HelloTriangle/HelloTriangle.vert.dxil.c"
#undef INLINED_FILE_INCLUSION_NAME

#define INLINED_FILE_INCLUSION_NAME k_MZNT_Internal_Dx12HelloTriangleMS
#include "../Shaders/HelloTriangle/HelloTriangle.mesh.dxil.c"
#undef INLINED_FILE_INCLUSION_NAME

#define INLINED_FILE_INCLUSION_NAME k_MZNT_Internal_Dx12HelloTriangleFS
#include "../Shaders/HelloTriangle/HelloTriangle.frag.dxil.c"
#undef INLINED_FILE_INCLUSION_NAME

#define INLINED_FILE_INCLUSION_NAME k_MZNT_Internal_Dx12FullScreenBlitVS
#include "../Shaders/FullScreenBlit/FullScreenBlit.vert.dxil.c"
#undef INLINED_FILE_INCLUSION_NAME

#define INLINED_FILE_INCLUSION_NAME k_MZNT_Internal_Dx12FullScreenBlitFS
#include "../Shaders/FullScreenBlit/FullScreenBlit.frag.dxil.c"
#undef INLINED_FILE_INCLUSION_NAME

static const DXGI_FORMAT k_MZNT_Internal_PreferredDx12ColourAttchFormat  = DXGI_FORMAT_R16G16B16A16_FLOAT;
static const DXGI_FORMAT k_MZNT_Internal_PreferredDx12DepthAttchFormat   = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

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

    // info queue setup
    {
        ID3D12InfoQueue* infoQueue = nil;
        if (SUCCEEDED(output->device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
        {
            infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
            infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);

            // hide some annoying warnings, like clearing render target with a different value than initialised from
            // the day that clearing a render target with a different colour than the "optimised" one becomes my bottleneck,
            // i'll leave game development forever and get a boring lifeless finance job or something

            D3D12_MESSAGE_ID hide[] = {
                D3D12_MESSAGE_ID_CREATEDEVICE_DEBUG_LAYER_STARTUP_OPTIONS,
                D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
            };

            D3D12_INFO_QUEUE_FILTER filter = { };
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;

            infoQueue->AddStorageFilterEntries(&filter);

            ID3D12InfoQueue1* iq1 = nil;
            if (SUCCEEDED(output->device->QueryInterface(IID_PPV_ARGS(&iq1))))
            {
                DWORD cookie = 0;
                iq1->RegisterMessageCallback(MZNT_Internal_GetDx12DebugCallback(), D3D12_MESSAGE_CALLBACK_FLAG_NONE, nil, &cookie);
                output->dbgCallbackCookie = cookie;

                iq1->Release();
            }

            infoQueue->Release();
        }
    }

    // queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = { };
    queueDesc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    MZNT_INTERNAL_DX12_CHECKED_CALL(output->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&(output->cmdQueue))));

    // allocator
    D3D12MA::ALLOCATOR_DESC allocDesc = { };
    allocDesc.pDevice = output->device;
    allocDesc.pAdapter = output->adapter;
    MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12MA::CreateAllocator(&allocDesc, &(output->d3d12maAllocator)));

    return output;
}

b8 MZNT_WaitTillRendererIdle_DirectX12(MZNT_DirectX12Renderer* renderer)
{
    if (!renderer || !(renderer->device) || !(renderer->cmdQueue)) return false;

    ID3D12Fence* fence = nil;
    MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

    MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->cmdQueue->Signal(fence, 1));

    if (fence->GetCompletedValue() < 1)
    {
        HANDLE fenceEvt = CreateEventA(nil, FALSE, FALSE, nil);
        MZNT_INTERNAL_DX12_CHECKED_CALL(fence->SetEventOnCompletion(1, fenceEvt));
        WaitForSingleObject(fenceEvt, INFINITE);
        CloseHandle(fenceEvt);
    }

    fence->Release();

    return true;
}

b8 MZNT_DestroyRenderer_DirectX12(MZNT_DirectX12Renderer* renderer, PNSLR_Allocator tempAllocator)
{
    if (!renderer) return false;

    MZNT_WaitTillRendererIdle_DirectX12(renderer);

    renderer->d3d12maAllocator->Release();

    renderer->cmdQueue->Release();

    ID3D12InfoQueue* iq = nil;
    if (SUCCEEDED(renderer->device->QueryInterface(IID_PPV_ARGS(&iq))))
    {
        ID3D12InfoQueue1* iq1 = nil;
        if (SUCCEEDED(iq->QueryInterface(IID_PPV_ARGS(&iq1))))
        {
            iq1->UnregisterMessageCallback(renderer->dbgCallbackCookie);
            iq1->Release();
        }

        iq->Release();
    }

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

static void MZNT_Internal_CreateFrameBufferAndViews(MZNT_DirectX12RendererSurface* surface)
{
    // swapchain views
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = surface->swapchainRtvHeap->GetCPUDescriptorHandleForHeapStart();
        for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        {
            ID3D12Resource* backBuffer;
            MZNT_INTERNAL_DX12_CHECKED_CALL(surface->swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));
            surface->renderer->device->CreateRenderTargetView(backBuffer, nil, rtvHandle);
            rtvHandle.ptr += surface->swapchainRtvDescriptorSize;

            surface->swapchainRTs[i] = backBuffer;
        }
    }

    // screen buffer
    {
        D3D12_RESOURCE_DESC colourDesc = { };
        colourDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        colourDesc.Width            = surface->swapchainWidth;
        colourDesc.Height           = surface->swapchainHeight;
        colourDesc.DepthOrArraySize = 1;
        colourDesc.MipLevels        = 1;
        colourDesc.Format           = k_MZNT_Internal_PreferredDx12ColourAttchFormat;
        colourDesc.SampleDesc.Count = 1;
        colourDesc.Flags            = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

        D3D12_CLEAR_VALUE colourClearValue = { };
        colourClearValue.Format   = k_MZNT_Internal_PreferredDx12ColourAttchFormat;
        colourClearValue.Color[0] = 0.0f;
        colourClearValue.Color[1] = 0.0f;
        colourClearValue.Color[2] = 0.0f;
        colourClearValue.Color[3] = 1.0f;

        D3D12MA::ALLOCATION_DESC allocationDesc = { };
        allocationDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

        for (i32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        {
            MZNT_INTERNAL_DX12_CHECKED_CALL(surface->renderer->d3d12maAllocator->CreateResource(
                &allocationDesc,
                &colourDesc,
                D3D12_RESOURCE_STATE_RENDER_TARGET,
                &colourClearValue,
                &(surface->screenBufferAllocations[i]),
                IID_PPV_ARGS(&(surface->screenBuffer[i]))
            ));
        }
    }

    // screen buffer rt views
    {
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = { };
        rtvDesc.Format             = k_MZNT_Internal_PreferredDx12ColourAttchFormat;
        rtvDesc.ViewDimension      = D3D12_RTV_DIMENSION_TEXTURE2D;
        rtvDesc.Texture2D.MipSlice = 0;

        D3D12_CPU_DESCRIPTOR_HANDLE svHandle = surface->svRtvHeap->GetCPUDescriptorHandleForHeapStart();
        for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        {
            surface->renderer->device->CreateRenderTargetView(surface->screenBuffer[i], &rtvDesc, svHandle);
            svHandle.ptr += surface->svRtvDescriptorSize;
        }
    }

    // screen buffer sr views
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
        srvDesc.Format                    = k_MZNT_Internal_PreferredDx12ColourAttchFormat;
        srvDesc.ViewDimension             = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Texture2D.MipLevels       = 1;
        srvDesc.Texture2D.MostDetailedMip = 0;

        D3D12_CPU_DESCRIPTOR_HANDLE svHandle = surface->svSrvHeap->GetCPUDescriptorHandleForHeapStart();
        for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        {
            surface->renderer->device->CreateShaderResourceView(surface->screenBuffer[i], &srvDesc, svHandle);
            svHandle.ptr += surface->svSrvDescriptorSize;
        }
    }

    // depth stencil buffer
    {
        D3D12_RESOURCE_DESC depthDesc = { };
        depthDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depthDesc.Width            = surface->swapchainWidth;
        depthDesc.Height           = surface->swapchainHeight;
        depthDesc.DepthOrArraySize = 1;
        depthDesc.MipLevels        = 1;
        depthDesc.Format           = k_MZNT_Internal_PreferredDx12DepthAttchFormat;
        depthDesc.SampleDesc.Count = 1;
        depthDesc.Flags            = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        D3D12_CLEAR_VALUE depthClearValue = { };
        depthClearValue.Format             = k_MZNT_Internal_PreferredDx12DepthAttchFormat;
        depthClearValue.DepthStencil.Depth = 1.0f; // stencil is 0-init

        D3D12MA::ALLOCATION_DESC allocationDesc = { };
        allocationDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

        for (i32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        {
            MZNT_INTERNAL_DX12_CHECKED_CALL(surface->renderer->d3d12maAllocator->CreateResource(
                &allocationDesc,
                &depthDesc,
                D3D12_RESOURCE_STATE_DEPTH_WRITE,
                &depthClearValue,
                &(surface->depthBufferAllocations[i]),
                IID_PPV_ARGS(&(surface->depthBuffer[i]))
            ));
        }
    }

    // depth stencil views
    {
        D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = { };
        dsvDesc.Format             = k_MZNT_Internal_PreferredDx12DepthAttchFormat;
        dsvDesc.ViewDimension      = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;

        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = surface->dsvHeap->GetCPUDescriptorHandleForHeapStart();
        for (i32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        {
            surface->renderer->device->CreateDepthStencilView(surface->depthBuffer[i], &dsvDesc, dsvHandle);
            dsvHandle.ptr += surface->dsvDescriptorSize;
        }
    }
}

MZNT_DirectX12RendererSurface* MZNT_CreateRendererSurfaceFromWindow_DirectX12(MZNT_DirectX12Renderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator)
{
    MZNT_DirectX12RendererSurface* output = PNSLR_New(MZNT_DirectX12RendererSurface, renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    if (!output) FORCE_DBG_TRAP;

    output->parent.type = MZNT_RendererType_DirectX12;
    output->renderer    = renderer;

    // swapchain rtv heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = { };
        heapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heapDesc.NumDescriptors = MZNT_NUM_FRAMES_IN_FLIGHT;
        heapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&(output->swapchainRtvHeap))));

        output->swapchainRtvDescriptorSize = renderer->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

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

    // screenbuffer rtv heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = { };
        heapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heapDesc.NumDescriptors = MZNT_NUM_FRAMES_IN_FLIGHT;
        heapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&(output->svRtvHeap))));

        output->svRtvDescriptorSize = renderer->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    // screenbuffer srv heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = { };
        heapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heapDesc.NumDescriptors = MZNT_NUM_FRAMES_IN_FLIGHT;
        heapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&(output->svSrvHeap))));

        output->svSrvDescriptorSize = renderer->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    // dsv heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = { };
        dsvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvHeapDesc.NumDescriptors = MZNT_NUM_FRAMES_IN_FLIGHT;
        dsvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&(output->dsvHeap))));

        output->dsvDescriptorSize = renderer->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    }

    MZNT_Internal_CreateFrameBufferAndViews(output);

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
    output->nextFenceValue = U64_MAX;
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

    MZNT_WaitTillRendererIdle_DirectX12(surface->renderer);

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

    // depth-stencil stuff
    for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
    {
        surface->depthBuffer[i]->Release();
        surface->depthBufferAllocations[i]->Release();
    }
    surface->dsvHeap->Release();

    // screen buffer stuff
    for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
    {
        surface->screenBuffer[i]->Release();
        surface->screenBufferAllocations[i]->Release();
    }
    surface->svSrvHeap->Release();
    surface->svRtvHeap->Release();

    // swapchain
    for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
    {
        surface->swapchainRTs[i]->Release();
    }
    surface->swapchain->Release();
    surface->swapchainRtvHeap->Release();

    PNSLR_Delete(surface, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    return true;
}

b8 MZNT_ResizeRendererSurface_DirectX12(MZNT_DirectX12RendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator)
{
    if (!surface) return false;
    if (!surface->renderer) FORCE_DBG_TRAP;

    MZNT_WaitTillRendererIdle_DirectX12(surface->renderer);

    // release render target views
    for (u32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
    {
        surface->depthBuffer[i]->Release();
        surface->depthBufferAllocations[i]->Release();
        surface->screenBuffer[i]->Release();
        surface->screenBufferAllocations[i]->Release();
        surface->swapchainRTs[i]->Release();
    }

    // resize buffers
    MZNT_INTERNAL_DX12_CHECKED_CALL(surface->swapchain->ResizeBuffers(MZNT_NUM_FRAMES_IN_FLIGHT, width, height, surface->swapchainFormat, 0));

    // update height/width
    {
        DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
        surface->swapchain->GetDesc1(&swapchainDesc);
        surface->swapchainWidth  = swapchainDesc.Width;
        surface->swapchainHeight = swapchainDesc.Height;
    }

    MZNT_Internal_CreateFrameBufferAndViews(surface);

    return true;
}

MZNT_DirectX12RendererCommandBuffer* MZNT_BeginFrame_DirectX12(MZNT_DirectX12RendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator)
{
    if (!surface) return nil;
    if (!surface->renderer) FORCE_DBG_TRAP;

    surface->nextFenceValue++;
    surface->frameFenceValues[surface->curFrame] = surface->nextFenceValue;
    surface->curFrame = surface->swapchain->GetCurrentBackBufferIndex();

    if (surface->fence->GetCompletedValue() < surface->frameFenceValues[surface->curFrame])
    {
        MZNT_INTERNAL_DX12_CHECKED_CALL(surface->fence->SetEventOnCompletion(surface->frameFenceValues[surface->curFrame], surface->fenceEvent));
        WaitForSingleObject(surface->fenceEvent, INFINITE);
    }

    MZNT_DirectX12RendererCommandBuffer& cmdBuffer = surface->commandBuffers[surface->curFrame];
    cmdBuffer.cmdAllocator->Reset();
    cmdBuffer.cmdList->Reset(cmdBuffer.cmdAllocator, nil);

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

    // bind screen buffer and depth buffer
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtv = surface->svRtvHeap->GetCPUDescriptorHandleForHeapStart(); // if you store the RTVs
        rtv.ptr += surface->curFrame * surface->svRtvDescriptorSize;

        D3D12_CPU_DESCRIPTOR_HANDLE dsv = surface->dsvHeap->GetCPUDescriptorHandleForHeapStart();
        dsv.ptr += surface->curFrame * surface->dsvDescriptorSize;

        cmdBuffer.cmdList->OMSetRenderTargets(1, &rtv, FALSE, &dsv);

        float clearColor[4] = {r, g, b, a};
        cmdBuffer.cmdList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
        cmdBuffer.cmdList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
    }

    return &cmdBuffer;
}

b8 MZNT_EndFrame_DirectX12(MZNT_DirectX12RendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    if (!surface) return false;
    if (!surface->renderer) FORCE_DBG_TRAP;

    MZNT_DirectX12RendererCommandBuffer& cmdBuffer = surface->commandBuffers[surface->curFrame];

    // screenbuffer: rt -> srv, swapchain: present -> rt
    {
        D3D12_RESOURCE_BARRIER barriers[2] = { };

        barriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(
            surface->screenBuffer[surface->curFrame],
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        barriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(
            surface->swapchainRTs[surface->curFrame],
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET);

        cmdBuffer.cmdList->ResourceBarrier(2, barriers);
    }

    // bind swapchain to output
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtv = surface->swapchainRtvHeap->GetCPUDescriptorHandleForHeapStart(); // if you store the RTVs
        rtv.ptr += surface->curFrame * surface->swapchainRtvDescriptorSize;

        cmdBuffer.cmdList->OMSetRenderTargets(1, &rtv, FALSE, nil);
    }

    // screenbuffer: srv -> rt, swapchain: rt -> present
    {
        D3D12_RESOURCE_BARRIER barriers[2] = { };

        barriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(
            surface->screenBuffer[surface->curFrame],
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            D3D12_RESOURCE_STATE_RENDER_TARGET);

        barriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(
            surface->swapchainRTs[surface->curFrame],
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT);

        cmdBuffer.cmdList->ResourceBarrier(2, barriers);
    }

    cmdBuffer.cmdList->Close();

    // submit
    ID3D12CommandList* cmdLists[] = { cmdBuffer.cmdList };
    surface->renderer->cmdQueue->ExecuteCommandLists(1, cmdLists);

    // present
    MZNT_INTERNAL_DX12_CHECKED_CALL(surface->swapchain->Present(1, 0));

    MZNT_INTERNAL_DX12_CHECKED_CALL(surface->renderer->cmdQueue->Signal(surface->fence, surface->nextFenceValue + 1));
    return true;
}

#endif
