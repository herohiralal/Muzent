#define MZNT_IMPLEMENTATION
#include "ShaderCompiler.h"

struct MZNT_Internal_ShaderCompiler
{
    PNSLR_DynamicLibrary lib;
    PNSLR_Allocator      allocator;
    IDxcUtils*           dxcUtils;
    IDxcCompiler3*       dxcCompiler;
    IDxcIncludeHandler*  dxcIncludeHandler;
};

MZNT_ShaderCompiler MZNT_CreateShaderCompiler(MZNT_ShaderCompilerConfiguration cfg, PNSLR_Allocator tempAllocator)
{
    #if PNSLR_WINDOWS
    utf8str libName = PNSLR_StringLiteral("dxcompiler.dll");
    #elif PNSLR_LINUX
    utf8str libName = PNSLR_StringLiteral("libdxcompiler.so");
    #else
    {
        libName = libName;
        PNSLR_LogW(PNSLR_StringLiteral("shader compiler unsupported on the current platform"), PNSLR_GET_LOC());
        return MZNT_ShaderCompiler { };
    }
    #endif

    #if PNSLR_WINDOWS || PNSLR_LINUX
    {
        if (!cfg.libSearchDir.path.count || !cfg.libSearchDir.path.data)
            return MZNT_ShaderCompiler { };

        PNSLR_Path library = PNSLR_GetPathForChildFile(cfg.libSearchDir, libName, tempAllocator);

        MZNT_Internal_ShaderCompiler* outputPtr = Panshilar::NewT<MZNT_Internal_ShaderCompiler>(cfg.allocator, PNSLR_GET_LOC());
        if (!outputPtr) return MZNT_ShaderCompiler { };

        MZNT_Internal_ShaderCompiler& output = *outputPtr;

        output.lib = PNSLR_LoadDynamicLibrary(library);
        if (!output.lib.handle)
        {
            PNSLR_LogEf(PNSLR_StringLiteral("could not load $"), PNSLR_FmtArgs(PNSLR_FmtString(library.path)), PNSLR_GET_LOC());
            Panshilar::DeleteT(outputPtr, cfg.allocator, PNSLR_GET_LOC());
            return MZNT_ShaderCompiler { };
        }

        DxcCreateInstanceProc createInstance = (DxcCreateInstanceProc) PNSLR_GetDynamicLibraryFunction(output.lib, PNSLR_StringLiteral("DxcCreateInstance"));
        if (!createInstance)
        {
            PNSLR_LogEf(PNSLR_StringLiteral("failed to load shader instance creation function from $"), PNSLR_FmtArgs(PNSLR_FmtString(library.path)), PNSLR_GET_LOC());
            PNSLR_UnloadDynamicLibrary(output.lib);
            Panshilar::DeleteT(outputPtr, cfg.allocator, PNSLR_GET_LOC());
            return MZNT_ShaderCompiler { };
        }

        if (!SUCCEEDED(createInstance(CLSID_DxcUtils, IID_PPV_ARGS(&output.dxcUtils))))
            FORCE_DBG_TRAP;

        if (!SUCCEEDED(createInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&output.dxcCompiler))))
            FORCE_DBG_TRAP;

        output.dxcUtils->CreateDefaultIncludeHandler(&output.dxcIncludeHandler);

        return MZNT_ShaderCompiler { outputPtr };
    }
    #endif
}

void MZNT_DestroyShaderCompiler(MZNT_ShaderCompiler compiler, PNSLR_Allocator tempAllocator)
{
    #if PNSLR_WINDOWS || PNSLR_LINUX
    {
        if (!compiler.handle) return;

        MZNT_Internal_ShaderCompiler& c = *(MZNT_Internal_ShaderCompiler*) compiler.handle;

        if (c.dxcIncludeHandler) c.dxcIncludeHandler->Release();
        if (c.dxcCompiler) c.dxcCompiler->Release();
        if (c.dxcUtils) c.dxcUtils->Release();
        if (c.lib.handle) PNSLR_UnloadDynamicLibrary(c.lib);

        Panshilar::DeleteT(&c, c.allocator, PNSLR_GET_LOC());
    }
    #endif
}
