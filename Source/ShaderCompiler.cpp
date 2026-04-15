#define MZNT_IMPLEMENTATION
#include "ShaderCompilerPrivate.h"

MZNT_Internal_ShaderCompiler MZNT_Internal_CreateShaderCompiler(PNSLR_Path libSearchDir, PNSLR_Allocator tempAllocator)
{
    #if PNSLR_WINDOWS
    utf8str libName = PNSLR_StringLiteral("dxcompiler.dll");
    #elif PNSLR_LINUX
    utf8str libName = PNSLR_StringLiteral("libdxcompiler.so");
    #else
    {
        libName = libName;
        PNSLR_LogW(PNSLR_StringLiteral("shader compiler unsupported on the current platform"), PNSLR_GET_LOC());
        return MZNT_Internal_ShaderCompiler { };
    }
    #endif

    #if PNSLR_WINDOWS || PNSLR_LINUX
    {
        if (!libSearchDir.path.count || !libSearchDir.path.data)
            return MZNT_Internal_ShaderCompiler { };

        PNSLR_Path library = PNSLR_GetPathForChildFile(libSearchDir, libName, tempAllocator);

        MZNT_Internal_ShaderCompiler output = { };
        output.lib = PNSLR_LoadDynamicLibrary(library);
        if (!output.lib.handle)
        {
            PNSLR_LogEf(PNSLR_StringLiteral("could not load $"), PNSLR_FmtArgs(PNSLR_FmtString(library.path)), PNSLR_GET_LOC());
            return output;
        }

        DxcCreateInstanceProc createInstance = (DxcCreateInstanceProc) PNSLR_GetDynamicLibraryFunction(output.lib, PNSLR_StringLiteral("DxcCreateInstance"));
        if (!createInstance)
        {
            PNSLR_LogEf(PNSLR_StringLiteral("failed to load shader instance creation function from $"), PNSLR_FmtArgs(PNSLR_FmtString(library.path)), PNSLR_GET_LOC());
            return output;
        }

        if (!SUCCEEDED(createInstance(CLSID_DxcUtils, IID_PPV_ARGS(&output.dxcUtils))))
            FORCE_DBG_TRAP;

        if (!SUCCEEDED(createInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&output.dxcCompiler))))
            FORCE_DBG_TRAP;

        output.dxcUtils->CreateDefaultIncludeHandler(&output.dxcIncludeHandler);

        return output;
    }
    #endif
}

void MZNT_Internal_DestroyShaderCompiler(MZNT_Internal_ShaderCompiler c, PNSLR_Allocator tempAllocator)
{
    #if PNSLR_WINDOWS || PNSLR_LINUX
    {
        if (c.dxcIncludeHandler) c.dxcIncludeHandler->Release();
        if (c.dxcCompiler) c.dxcCompiler->Release();
        if (c.dxcUtils) c.dxcUtils->Release();
        if (c.lib.handle) PNSLR_UnloadDynamicLibrary(c.lib);
    }
    #endif
}
