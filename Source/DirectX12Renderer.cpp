#define MZNT_IMPLEMENTATION
#include "DirectX12Renderer.h"
#if MZNT_DX12

static inline void MZNT_Internal_LogDx12ResultOnFailure(HRESULT result, utf8str fnCall, PNSLR_SourceCodeLocation loc)
{
    if (SUCCEEDED(result)) return;
    utf8str message = {0};
    switch (result)
    {
        case D3D12_ERROR_ADAPTER_NOT_FOUND: message = PNSLR_StringLiteral("The specified cached PSO was created on a different adapter and cannot be reused on the current adapter."); break;
        case D3D12_ERROR_DRIVER_VERSION_MISMATCH: message = PNSLR_StringLiteral("The specified cached PSO was created on a different driver version and cannot be reused on the current adapter."); break;
        case DXGI_ERROR_INVALID_CALL: message = PNSLR_StringLiteral("The method call is invalid. For example, a method's parameter may not be a valid pointer."); break;
        case DXGI_ERROR_WAS_STILL_DRAWING: message = PNSLR_StringLiteral("The previous blit operation that is transferring information to or from this surface is incomplete."); break;
        case E_FAIL: message = PNSLR_StringLiteral("Attempted to create a device with the debug layer enabled and the layer is not installed."); break;
        case E_INVALIDARG: message = PNSLR_StringLiteral("An invalid parameter was passed to the returning function."); break;
        case E_OUTOFMEMORY: message = PNSLR_StringLiteral("Direct3D could not allocate sufficient memory to complete the call."); break;
        case E_NOTIMPL: message = PNSLR_StringLiteral("The method call isn't implemented with the passed parameter combination."); break;
        case S_FALSE: message = PNSLR_StringLiteral("Alternate success value, indicating a successful but nonstandard completion (the precise meaning depends on context)."); break;
        default: message = PNSLR_StringLiteral("An unknown error occurred."); break;
    }

    if (result != S_OK)
    {
        PNSLR_LogEf(PNSLR_StringLiteral("DirectX 12 error: $ from $"),
                    PNSLR_FmtArgs(
                        PNSLR_FmtString(message),
                        PNSLR_FmtString(fnCall)
                    ),
                    loc);

        FORCE_DBG_TRAP;
    }
}

#define MZNT_INTERNAL_DX12_CHECKED_CALL(call) \
    MZNT_Internal_LogDx12ResultOnFailure((call), PNSLR_StringLiteral(#call), PNSLR_GET_LOC())

MZNT_DirectX12Renderer* MZNT_CreateRenderer_DirectX12(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator)
{
    UINT dxgiFactoryFlags = 0;

    if (PNSLR_DBG)
    {
        ID3D12Debug* debugController;
        MZNT_INTERNAL_DX12_CHECKED_CALL(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
        debugController->EnableDebugLayer();

        ID3D12Debug1* dbg1;
        if (S_OK == debugController->QueryInterface(IID_PPV_ARGS(&dbg1)))
        {
            dbg1->SetEnableGPUBasedValidation(true);
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }

    IDXGIFactory4* factory;
    MZNT_INTERNAL_DX12_CHECKED_CALL(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    return nil;
}

b8 MZNT_DestroyRenderer_DirectX12(MZNT_DirectX12Renderer* renderer, PNSLR_Allocator tempAllocator)
{
    return false;
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
