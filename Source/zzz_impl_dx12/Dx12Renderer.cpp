#define MZNT_IMPLEMENTATION
#include "Dx12Renderer.h"
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

static void __stdcall MZNT_Internal_Dx12DebugCallback(
    D3D12_MESSAGE_CATEGORY category,
    D3D12_MESSAGE_SEVERITY severity,
    D3D12_MESSAGE_ID id,
    LPCSTR pDescription,
    void* pContext)
{
    utf8str categoryStr = { };
    switch (category)
    {
        case D3D12_MESSAGE_CATEGORY_APPLICATION_DEFINED:   categoryStr = PNSLR_StringLiteral("[ApplicationDefined]");   break;
        case D3D12_MESSAGE_CATEGORY_MISCELLANEOUS:         categoryStr = PNSLR_StringLiteral("[Miscellaneous]");        break;
        case D3D12_MESSAGE_CATEGORY_INITIALIZATION:        categoryStr = PNSLR_StringLiteral("[Initialization]");       break;
        case D3D12_MESSAGE_CATEGORY_CLEANUP:               categoryStr = PNSLR_StringLiteral("[Cleanup]");              break;
        case D3D12_MESSAGE_CATEGORY_COMPILATION:           categoryStr = PNSLR_StringLiteral("[Compilation]");          break;
        case D3D12_MESSAGE_CATEGORY_STATE_CREATION:        categoryStr = PNSLR_StringLiteral("[StateCreation]");        break;
        case D3D12_MESSAGE_CATEGORY_STATE_SETTING:         categoryStr = PNSLR_StringLiteral("[StateSetting]");         break;
        case D3D12_MESSAGE_CATEGORY_STATE_GETTING:         categoryStr = PNSLR_StringLiteral("[StateGetting]");         break;
        case D3D12_MESSAGE_CATEGORY_RESOURCE_MANIPULATION: categoryStr = PNSLR_StringLiteral("[ResourceManipulation]"); break;
        case D3D12_MESSAGE_CATEGORY_EXECUTION:             categoryStr = PNSLR_StringLiteral("[Execution]");            break;
        case D3D12_MESSAGE_CATEGORY_SHADER:                categoryStr = PNSLR_StringLiteral("[Shader]");               break;
        default:                                           categoryStr = PNSLR_StringLiteral("[Unknown]");              break;
    }

    PNSLR_LoggerLevel lvl = 0;
    switch (severity)
    {
        case D3D12_MESSAGE_SEVERITY_CORRUPTION: lvl = PNSLR_LoggerLevel_Critical; break;
        case D3D12_MESSAGE_SEVERITY_ERROR:      lvl = PNSLR_LoggerLevel_Error;    break;
        case D3D12_MESSAGE_SEVERITY_WARNING:    lvl = PNSLR_LoggerLevel_Warn;     break;
        case D3D12_MESSAGE_SEVERITY_INFO:       lvl = PNSLR_LoggerLevel_Info;     break;
        case D3D12_MESSAGE_SEVERITY_MESSAGE:    lvl = PNSLR_LoggerLevel_Debug;    break;
        default:                                lvl = PNSLR_LoggerLevel_Error;    break;
    }

    PNSLR_Logf(lvl, PNSLR_StringLiteral("D3D12 INFO QUEUE: $ $"),
        PNSLR_FmtArgs(
            PNSLR_FmtCString((cstring) pDescription),
            PNSLR_FmtString(categoryStr),
        ),
        PNSLR_GET_LOC()
    );
}

static inline void MZNT_Internal_LogDx12ResultOnFailure(HRESULT result, utf8str fnCall, PNSLR_SourceCodeLocation loc)
{
    if (SUCCEEDED(result)) return;
    utf8str message = {0};
    switch (result)
    {
        case D3D12_ERROR_ADAPTER_NOT_FOUND: message = Panshilar::StringLiteral("\"The specified cached PSO was created on a different adapter and cannot be reused on the current adapter.\""); break;
        case D3D12_ERROR_DRIVER_VERSION_MISMATCH: message = Panshilar::StringLiteral("\"The specified cached PSO was created on a different driver version and cannot be reused on the current adapter.\""); break;
        case DXGI_ERROR_INVALID_CALL: message = Panshilar::StringLiteral("\"The method call is invalid. For example, a method's parameter may not be a valid pointer.\""); break;
        case DXGI_ERROR_WAS_STILL_DRAWING: message = Panshilar::StringLiteral("\"The previous blit operation that is transferring information to or from this surface is incomplete.\""); break;
        case E_FAIL: message = Panshilar::StringLiteral("\"Attempted to create a device with the debug layer enabled and the layer is not installed.\""); break;
        case E_INVALIDARG: message = Panshilar::StringLiteral("\"An invalid parameter was passed to the returning function.\""); break;
        case E_OUTOFMEMORY: message = Panshilar::StringLiteral("\"Direct3D could not allocate sufficient memory to complete the call.\""); break;
        case E_NOTIMPL: message = Panshilar::StringLiteral("\"The method call isn't implemented with the passed parameter combination.\""); break;
        case S_FALSE: message = Panshilar::StringLiteral("\"Alternate success value, indicating a successful but nonstandard completion (the precise meaning depends on context).\""); break;
        default: message = Panshilar::StringLiteral("\"An unknown error occurred.\""); break;
    }

    if (result != S_OK)
    {
        PNSLR_LogEf(Panshilar::StringLiteral("DirectX 12 error: $ from $."),
                    PNSLR_FmtArgs(
                        PNSLR_FmtString(message),
                        PNSLR_FmtString(fnCall)
                    ),
                    loc);

        FORCE_DBG_TRAP;
    }
}

static inline void MZNT_Internal_LogErrorBlobAndRelease(ID3DBlob* blob, utf8str objName, PNSLR_SourceCodeLocation loc)
{
    if (!blob) return;

    utf8str blobMsg = { };
    blobMsg.data  = (u8*) blob->GetBufferPointer();
    blobMsg.count = (i64) blob->GetBufferSize();

    if (blobMsg.data && blobMsg.data[blobMsg.count - 1] == '\0')
    {
        // trim null terminator from message for cleaner logging
        blobMsg.count -= 1;
    }

    PNSLR_LogEf(Panshilar::StringLiteral("[$]: $"),
                PNSLR_FmtArgs(
                    PNSLR_FmtString(objName),
                    PNSLR_FmtString(blobMsg)
                ),
                loc);

    blob->Release();
}

#define MZNT_INTERNAL_DX12_CHECKED_CALL(call) \
    MZNT_Internal_LogDx12ResultOnFailure((call), Panshilar::StringLiteral(#call), PNSLR_GET_LOC())

#define MZNT_INTERNAL_DX12_LOG_BLOB_AND_RELEASE(blob) \
    MZNT_Internal_LogErrorBlobAndRelease((blob), Panshilar::StringLiteral(#blob), PNSLR_GET_LOC())

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
                iq1->RegisterMessageCallback(MZNT_Internal_Dx12DebugCallback, D3D12_MESSAGE_CALLBACK_FLAG_NONE, nil, &cookie);
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

    // hello triangle shader
    {
        // root signature - vs
        {
            D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc = { };
            rootDesc.Version                    = D3D_ROOT_SIGNATURE_VERSION_1_0;
            rootDesc.Desc_1_0.NumParameters     = 0;
            rootDesc.Desc_1_0.pParameters       = nil;
            rootDesc.Desc_1_0.NumStaticSamplers = 0;
            rootDesc.Desc_1_0.pStaticSamplers   = nil;
            rootDesc.Desc_1_0.Flags             = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

            ID3DBlob* serializedDesc = nil;
            ID3DBlob* errorBlob = nil;
            MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12SerializeVersionedRootSignature(&rootDesc, &serializedDesc, &errorBlob));
            MZNT_INTERNAL_DX12_LOG_BLOB_AND_RELEASE(errorBlob);
            MZNT_INTERNAL_DX12_CHECKED_CALL(
                output->device->CreateRootSignature(
                    0,
                    serializedDesc->GetBufferPointer(),
                    serializedDesc->GetBufferSize(),
                    IID_PPV_ARGS(&(output->helloTriangleVertexShadedProgram.rootSignature))
                ));
            serializedDesc->Release();
        }

        // pso - vs
        {
            D3D12_INPUT_LAYOUT_DESC inputLayout = { };
            inputLayout.pInputElementDescs = nil;
            inputLayout.NumElements        = 0;

            D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { };
            psoDesc.pRootSignature                  = output->helloTriangleVertexShadedProgram.rootSignature;
            psoDesc.VS.pShaderBytecode              = k_MZNT_Internal_Dx12HelloTriangleVSContents;
            psoDesc.VS.BytecodeLength               = k_MZNT_Internal_Dx12HelloTriangleVSSize;
            psoDesc.PS.pShaderBytecode              = k_MZNT_Internal_Dx12HelloTriangleFSContents;
            psoDesc.PS.BytecodeLength               = k_MZNT_Internal_Dx12HelloTriangleFSSize;
            psoDesc.InputLayout.pInputElementDescs  = inputLayout.pInputElementDescs;
            psoDesc.InputLayout.NumElements         = inputLayout.NumElements;
            psoDesc.PrimitiveTopologyType           = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            psoDesc.RTVFormats[0]                   = k_MZNT_Internal_PreferredDx12ColourAttchFormat;
            psoDesc.NumRenderTargets                = 1;
            psoDesc.SampleDesc.Count                = 1;
            psoDesc.SampleMask                      = UINT_MAX;
            psoDesc.RasterizerState                 = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
            psoDesc.BlendState                      = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
            psoDesc.DepthStencilState               = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
            psoDesc.DSVFormat                       = k_MZNT_Internal_PreferredDx12DepthAttchFormat;

            MZNT_INTERNAL_DX12_CHECKED_CALL(output->device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&(output->helloTriangleVertexShadedProgram.pipelineState))));
        }

        // root signature - ms
        {
            D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc = { };
            rootDesc.Version                    = D3D_ROOT_SIGNATURE_VERSION_1_0;
            rootDesc.Desc_1_0.NumParameters     = 0;
            rootDesc.Desc_1_0.pParameters       = nil;
            rootDesc.Desc_1_0.NumStaticSamplers = 0;
            rootDesc.Desc_1_0.pStaticSamplers   = nil;
            rootDesc.Desc_1_0.Flags             = D3D12_ROOT_SIGNATURE_FLAG_NONE;

            ID3DBlob* serializedDesc = nil;
            ID3DBlob* errorBlob = nil;
            MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12SerializeVersionedRootSignature(&rootDesc, &serializedDesc, &errorBlob));
            MZNT_INTERNAL_DX12_LOG_BLOB_AND_RELEASE(errorBlob);
            MZNT_INTERNAL_DX12_CHECKED_CALL(
                output->device->CreateRootSignature(
                    0,
                    serializedDesc->GetBufferPointer(),
                    serializedDesc->GetBufferSize(),
                    IID_PPV_ARGS(&(output->helloTriangleMeshShadedProgram.rootSignature))
                ));
            serializedDesc->Release();
        }

        // pso - ms
        {
            D3DX12_MESH_SHADER_PIPELINE_STATE_DESC psoDesc = { };
            psoDesc.pRootSignature                  = output->helloTriangleMeshShadedProgram.rootSignature;
            psoDesc.MS.pShaderBytecode              = k_MZNT_Internal_Dx12HelloTriangleMSContents;
            psoDesc.MS.BytecodeLength               = k_MZNT_Internal_Dx12HelloTriangleMSSize;
            psoDesc.PS.pShaderBytecode              = k_MZNT_Internal_Dx12HelloTriangleFSContents;
            psoDesc.PS.BytecodeLength               = k_MZNT_Internal_Dx12HelloTriangleFSSize;
            psoDesc.PrimitiveTopologyType           = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            psoDesc.RTVFormats[0]                   = k_MZNT_Internal_PreferredDx12ColourAttchFormat;
            psoDesc.NumRenderTargets                = 1;
            psoDesc.SampleDesc.Count                = 1;
            psoDesc.SampleMask                      = UINT_MAX;
            psoDesc.RasterizerState                 = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
            psoDesc.BlendState                      = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
            psoDesc.DepthStencilState               = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
            psoDesc.DSVFormat                       = k_MZNT_Internal_PreferredDx12DepthAttchFormat;

            CD3DX12_PIPELINE_MESH_STATE_STREAM psoStream = psoDesc;

            D3D12_PIPELINE_STATE_STREAM_DESC streamDesc = { };
            streamDesc.pPipelineStateSubobjectStream = &psoStream;
            streamDesc.SizeInBytes                   = sizeof(psoStream);

            MZNT_INTERNAL_DX12_CHECKED_CALL(output->device->CreatePipelineState(&streamDesc, IID_PPV_ARGS(&(output->helloTriangleMeshShadedProgram.pipelineState))));
        }
    }

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

    renderer->helloTriangleMeshShadedProgram.pipelineState->Release();
    renderer->helloTriangleMeshShadedProgram.rootSignature->Release();

    renderer->helloTriangleVertexShadedProgram.pipelineState->Release();
    renderer->helloTriangleVertexShadedProgram.rootSignature->Release();

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

    // fullscreen blit shader
    {
        // root signature
        {
            D3D12_DESCRIPTOR_RANGE srcRange = { };
            srcRange.RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            srcRange.NumDescriptors                    = 1;
            srcRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

            D3D12_ROOT_PARAMETER srcParam = { };
            srcParam.ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            srcParam.ShaderVisibility                    = D3D12_SHADER_VISIBILITY_PIXEL;
            srcParam.DescriptorTable.NumDescriptorRanges = 1;
            srcParam.DescriptorTable.pDescriptorRanges   = &srcRange;

            D3D12_STATIC_SAMPLER_DESC linearSampler = { };
            linearSampler.Filter           = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
            linearSampler.AddressU         = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            linearSampler.AddressV         = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            linearSampler.AddressW         = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            linearSampler.BorderColor      = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
            linearSampler.MaxAnisotropy    = 1;
            linearSampler.ComparisonFunc   = D3D12_COMPARISON_FUNC_ALWAYS;
            linearSampler.MaxLOD           = D3D12_FLOAT32_MAX;
            linearSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

            D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc = { };
            rootDesc.Version                    = D3D_ROOT_SIGNATURE_VERSION_1_0;
            rootDesc.Desc_1_0.NumParameters     = 1;
            rootDesc.Desc_1_0.pParameters       = &srcParam;
            rootDesc.Desc_1_0.NumStaticSamplers = 1;
            rootDesc.Desc_1_0.pStaticSamplers   = &linearSampler;
            rootDesc.Desc_1_0.Flags             = D3D12_ROOT_SIGNATURE_FLAG_NONE;

            ID3DBlob* serializedDesc = nil;
            ID3DBlob* errorBlob = nil;
            MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12SerializeVersionedRootSignature(&rootDesc, &serializedDesc, &errorBlob));
            MZNT_INTERNAL_DX12_LOG_BLOB_AND_RELEASE(errorBlob);
            MZNT_INTERNAL_DX12_CHECKED_CALL(
                renderer->device->CreateRootSignature(
                    0,
                    serializedDesc->GetBufferPointer(),
                    serializedDesc->GetBufferSize(),
                    IID_PPV_ARGS(&(output->finalBlitShader.rootSignature))
                ));
            serializedDesc->Release();
        }

        // pso
        {
            D3D12_INPUT_LAYOUT_DESC inputLayout = { };
            inputLayout.pInputElementDescs = nil;
            inputLayout.NumElements        = 0;

            D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { };
            psoDesc.pRootSignature                  = output->finalBlitShader.rootSignature;
            psoDesc.VS.pShaderBytecode              = k_MZNT_Internal_Dx12FullScreenBlitVSContents;
            psoDesc.VS.BytecodeLength               = k_MZNT_Internal_Dx12FullScreenBlitVSSize;
            psoDesc.PS.pShaderBytecode              = k_MZNT_Internal_Dx12FullScreenBlitFSContents;
            psoDesc.PS.BytecodeLength               = k_MZNT_Internal_Dx12FullScreenBlitFSSize;
            psoDesc.InputLayout.pInputElementDescs  = inputLayout.pInputElementDescs;
            psoDesc.InputLayout.NumElements         = inputLayout.NumElements;
            psoDesc.PrimitiveTopologyType           = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            psoDesc.RTVFormats[0]                   = output->swapchainFormat;
            psoDesc.NumRenderTargets                = 1;
            psoDesc.SampleDesc.Count                = 1;
            psoDesc.SampleMask                      = UINT_MAX;
            psoDesc.RasterizerState                 = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
            psoDesc.BlendState                      = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
            psoDesc.DepthStencilState               = D3D12_DEPTH_STENCIL_DESC();
            psoDesc.DepthStencilState.DepthEnable   = false;
            psoDesc.DepthStencilState.StencilEnable = false;
            psoDesc.DSVFormat                       = DXGI_FORMAT_UNKNOWN;

            MZNT_INTERNAL_DX12_CHECKED_CALL(renderer->device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&(output->finalBlitShader.pipelineState))));
        }
    }

    return output;
}

b8 MZNT_DestroyRendererSurface_DirectX12(MZNT_DirectX12RendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    if (!surface) return false;

    MZNT_WaitTillRendererIdle_DirectX12(surface->renderer);

    surface->finalBlitShader.pipelineState->Release();
    surface->finalBlitShader.rootSignature->Release();

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

    // draw triangle - vs
    {
        cmdBuffer.cmdList->SetPipelineState(surface->renderer->helloTriangleVertexShadedProgram.pipelineState);
        cmdBuffer.cmdList->SetGraphicsRootSignature(surface->renderer->helloTriangleVertexShadedProgram.rootSignature);
        cmdBuffer.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        cmdBuffer.cmdList->IASetVertexBuffers(0, 0, nil);
        cmdBuffer.cmdList->IASetIndexBuffer(nil);
        cmdBuffer.cmdList->DrawInstanced(3, 1, 0, 0);
    }
    // draw triangle - ms
    {
        cmdBuffer.cmdList->SetPipelineState(surface->renderer->helloTriangleMeshShadedProgram.pipelineState);
        cmdBuffer.cmdList->SetGraphicsRootSignature(surface->renderer->helloTriangleMeshShadedProgram.rootSignature);
        cmdBuffer.cmdList->DispatchMesh(1, 1, 1);
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

    // cant do a copy texture, because the outputs are in different formats, so do a fullscreen blit instead
    {
        cmdBuffer.cmdList->SetPipelineState(surface->finalBlitShader.pipelineState);
        cmdBuffer.cmdList->SetGraphicsRootSignature(surface->finalBlitShader.rootSignature);

        cmdBuffer.cmdList->SetDescriptorHeaps(1, &surface->svSrvHeap);

        D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = surface->svSrvHeap->GetGPUDescriptorHandleForHeapStart();
        srvHandle.ptr += surface->curFrame * surface->svSrvDescriptorSize;
        cmdBuffer.cmdList->SetGraphicsRootDescriptorTable(0, srvHandle);
        cmdBuffer.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        cmdBuffer.cmdList->IASetVertexBuffers(0, 0, nil);
        cmdBuffer.cmdList->IASetIndexBuffer(nil);
        cmdBuffer.cmdList->DrawInstanced(3, 1, 0, 0);
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

MZNT_DirectX12SwapChain* MZNT_CreateSwapChainFromWindow_DirectX12(MZNT_DirectX12Renderer* renderer, MZNT_WindowHandle windowHandle, MZNT_SwapChainConfiguration cfg, PNSLR_Allocator tempAllocator)
{
    return nil;
}

b8 MZNT_ReconfigureSwapChain_DirectX12(MZNT_DirectX12SwapChain* swapChain, MZNT_SwapChainConfiguration cfg, PNSLR_Allocator tempAllocator)
{
    return false;
}

b8 MZNT_DestroySwapChain_DirectX12(MZNT_DirectX12SwapChain* swapChain, PNSLR_Allocator tempAllocator)
{
    return false;
}

MZNT_TextureFormat MZNT_GetSwapChainTextureFormat_DirectX12(MZNT_DirectX12SwapChain* swapChain)
{
     return MZNT_TextureFormat_Unknown;
}

u8 MZNT_IterateSwapChainAndGetIndex_DirectX12(MZNT_DirectX12SwapChain* swapChain)
{
    return U8_MAX;
}

#endif
