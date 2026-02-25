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

static const DXGI_FORMAT k_DVRPL_Internal_PreferredColourAttchFormat  = DXGI_FORMAT_R16G16B16A16_FLOAT;
static const DXGI_FORMAT k_DVRPL_Internal_PreferredDepthAttchFormat   = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

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
                continue;

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
            psoDesc.pRootSignature                 = output->triangleShader.rootSignature;
            psoDesc.VS.pShaderBytecode             = k_MZNT_Internal_TriangleShaderVSContents;
            psoDesc.VS.BytecodeLength              = k_MZNT_Internal_TriangleShaderVSSize;
            psoDesc.PS.pShaderBytecode             = k_MZNT_Internal_TriangleShaderPSContents;
            psoDesc.PS.BytecodeLength              = k_MZNT_Internal_TriangleShaderPSSize;
            psoDesc.InputLayout.pInputElementDescs = inputLayout.pInputElementDescs;
            psoDesc.InputLayout.NumElements        = inputLayout.NumElements;
            psoDesc.PrimitiveTopologyType          = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            psoDesc.RTVFormats[0]                  = k_DVRPL_Internal_PreferredColourAttchFormat;
            psoDesc.NumRenderTargets               = 1;
            psoDesc.DSVFormat                      = k_DVRPL_Internal_PreferredDepthAttchFormat;
            psoDesc.SampleDesc.Count               = 1;
            psoDesc.SampleMask                     = UINT_MAX;

            psoDesc.RasterizerState.FillMode              = D3D12_FILL_MODE_SOLID;
            psoDesc.RasterizerState.CullMode              = D3D12_CULL_MODE_BACK;
            psoDesc.RasterizerState.FrontCounterClockwise = false;
            psoDesc.RasterizerState.DepthBias             = D3D12_DEFAULT_DEPTH_BIAS;
            psoDesc.RasterizerState.DepthBiasClamp        = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
            psoDesc.RasterizerState.SlopeScaledDepthBias  = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
            psoDesc.RasterizerState.DepthClipEnable       = true;
            psoDesc.RasterizerState.MultisampleEnable     = false;
            psoDesc.RasterizerState.AntialiasedLineEnable = false;
            psoDesc.RasterizerState.ForcedSampleCount     = 0;
            psoDesc.RasterizerState.ConservativeRaster    = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

            psoDesc.BlendState.AlphaToCoverageEnable      = false;
            psoDesc.BlendState.IndependentBlendEnable     = false;
            for (u32 i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
            {
                D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = psoDesc.BlendState.RenderTarget[i];
                blendDesc.BlendEnable                     = false;
                blendDesc.LogicOpEnable                   = false;
                blendDesc.SrcBlend                        = D3D12_BLEND_ONE;
                blendDesc.DestBlend                       = D3D12_BLEND_ZERO;
                blendDesc.BlendOp                         = D3D12_BLEND_OP_ADD;
                blendDesc.SrcBlendAlpha                   = D3D12_BLEND_ONE;
                blendDesc.DestBlendAlpha                  = D3D12_BLEND_ZERO;
                blendDesc.BlendOpAlpha                    = D3D12_BLEND_OP_ADD;
                blendDesc.LogicOp                         = D3D12_LOGIC_OP_NOOP;
                blendDesc.RenderTargetWriteMask           = D3D12_COLOR_WRITE_ENABLE_ALL;
            }

            psoDesc.DepthStencilState.DepthEnable          = true;
            psoDesc.DepthStencilState.DepthWriteMask       = D3D12_DEPTH_WRITE_MASK_ALL;
            psoDesc.DepthStencilState.DepthFunc            = D3D12_COMPARISON_FUNC_LESS;
            psoDesc.DepthStencilState.StencilEnable        = false;
            psoDesc.DepthStencilState.StencilReadMask      = D3D12_DEFAULT_STENCIL_READ_MASK;
            psoDesc.DepthStencilState.StencilWriteMask     = D3D12_DEFAULT_STENCIL_WRITE_MASK;
            psoDesc.DepthStencilState.FrontFace.StencilFailOp      = D3D12_STENCIL_OP_KEEP;
            psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
            psoDesc.DepthStencilState.FrontFace.StencilPassOp      = D3D12_STENCIL_OP_KEEP;
            psoDesc.DepthStencilState.FrontFace.StencilFunc        = D3D12_COMPARISON_FUNC_ALWAYS;
            psoDesc.DepthStencilState.BackFace.StencilFailOp       = D3D12_STENCIL_OP_KEEP;
            psoDesc.DepthStencilState.BackFace.StencilDepthFailOp  = D3D12_STENCIL_OP_KEEP;
            psoDesc.DepthStencilState.BackFace.StencilPassOp       = D3D12_STENCIL_OP_KEEP;
            psoDesc.DepthStencilState.BackFace.StencilFunc         = D3D12_COMPARISON_FUNC_ALWAYS;

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

MZNT_DirectX12RendererSurface* MZNT_CreateRendererSurfaceFromWindow_DirectX12(MZNT_DirectX12Renderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator)
{
    MZNT_DirectX12RendererSurface* output = PNSLR_New(MZNT_DirectX12RendererSurface, renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    if (!output) FORCE_DBG_TRAP;

    output->parent.type = MZNT_RendererType_DirectX12;
    output->renderer    = renderer;

    return output;
}

b8 MZNT_DestroyRendererSurface_DirectX12(MZNT_DirectX12RendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    PNSLR_Delete(surface, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    return true;
}

b8 MZNT_ResizeRendererSurface_DirectX12(MZNT_DirectX12RendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator)
{
    return false;
}

MZNT_DirectX12RendererCommandBuffer* MZNT_BeginFrame_DirectX12(MZNT_DirectX12RendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator)
{
    return nil;
}

b8 MZNT_EndFrame_DirectX12(MZNT_DirectX12RendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    return false;
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
