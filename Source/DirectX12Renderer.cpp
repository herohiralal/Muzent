#define MZNT_IMPLEMENTATION
#include "DirectX12Renderer.h"
#if MZNT_DX12

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

MZNT_DirectX12Renderer* MZNT_CreateRenderer_DirectX12(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator)
{
    MZNT_DirectX12Renderer* output = PNSLR_New(MZNT_DirectX12Renderer, config.allocator, PNSLR_GET_LOC(), nil);
    if (!output) FORCE_DBG_TRAP;

    output->parent.type      = MZNT_RendererType_DirectX12;
    output->parent.allocator = config.allocator;
    output->parent.appHandle = config.appHandle;

    u32 dxgiFactoryFlags = 0;
    if (PNSLR_DBG)
    {
        MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12GetDebugInterface(IID_PPV_ARGS(&(output->dbgController))));
        output->dbgController->EnableDebugLayer();
        output->dbgController->SetEnableGPUBasedValidation(true);
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }

    MZNT_INTERNAL_DX12_CHECKED_CALL(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&(output->dxgiFactory))));

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
    }

    if (!output->adapter)
    {
        PNSLR_LogE(Panshilar::StringLiteral("No compatible DirectX 12 adapter found."), PNSLR_GET_LOC());
        FORCE_DBG_TRAP;
        return nil;
    }

    MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12CreateDevice(output->adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&(output->device))));

    D3D12_COMMAND_QUEUE_DESC queueDesc = { };
    queueDesc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    MZNT_INTERNAL_DX12_CHECKED_CALL(output->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&(output->cmdQueue))));

    D3D12MA::ALLOCATOR_DESC allocDesc = { };
    allocDesc.pDevice = output->device;
    allocDesc.pAdapter = output->adapter;
    MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12MA::CreateAllocator(&allocDesc, &MZNT_Internal_GetAllocator(output)));

    return nil;
}

b8 MZNT_DestroyRenderer_DirectX12(MZNT_DirectX12Renderer* renderer, PNSLR_Allocator tempAllocator)
{
    if (!renderer) return false;

    MZNT_Internal_GetAllocator(renderer)->Release();

    renderer->cmdQueue->Release();

    renderer->device->Release();

    renderer->adapter->Release();

    renderer->dxgiFactory->Release();

    if (renderer->dbgController)
    {
        renderer->dbgController->SetEnableGPUBasedValidation(false);
        renderer->dbgController->DisableDebugLayer();
        renderer->dbgController->Release();
    }

    PNSLR_Delete(renderer, renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    return true;
}

MZNT_DirectX12RendererSurface* MZNT_CreateRendererSurfaceFromWindow_DirectX12(MZNT_DirectX12Renderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator)
{
    return nil;
}

b8 MZNT_DestroyRendererSurface_DirectX12(MZNT_DirectX12RendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    return false;
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
