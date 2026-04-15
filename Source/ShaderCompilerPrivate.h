#ifndef MZNT_SHADER_COMPILER_PVT_H // ==============================================
#define MZNT_SHADER_COMPILER_PVT_H
#include "__Prelude.h"
#include "Renderer.h"
EXTERN_C_BEGIN

#ifndef __cplusplus
typedef struct IDxcCompiler3      IDxcCompiler3;
typedef struct IDxcUtils          IDxcUtils;
typedef struct IDxcIncludeHandler IDxcIncludeHandler;
#endif

typedef struct MZNT_Internal_ShaderCompiler
{
    PNSLR_DynamicLibrary lib;
    IDxcUtils*           dxcUtils;
    IDxcCompiler3*       dxcCompiler;
    IDxcIncludeHandler*  dxcIncludeHandler;
} MZNT_Internal_ShaderCompiler;

MZNT_Internal_ShaderCompiler MZNT_Internal_CreateShaderCompiler(PNSLR_Path libSearchDir, PNSLR_Allocator tempAllocator);
void MZNT_Internal_DestroyShaderCompiler(MZNT_Internal_ShaderCompiler c, PNSLR_Allocator tempAllocator);

EXTERN_C_END
#endif
