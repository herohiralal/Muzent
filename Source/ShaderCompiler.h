#ifndef MZNT_SHADER_COMPILER_H // ==================================================
#define MZNT_SHADER_COMPILER_H
#include "__Prelude.h"
#include "Renderer.h"
EXTERN_C_BEGIN

/**
 * The configuration to use when constructing the shader compiler.
 */
typedef struct MZNT_ShaderCompilerConfiguration
{
    PNSLR_Path      libSearchDir;
    PNSLR_Allocator allocator;
} MZNT_ShaderCompilerConfiguration;

/**
 * Handle to the shader compiler. This compiler can convert shader source files into corresponding
 * bytecode, based on the renderer.
 * - Vulkan: HLSL -> SPIR-V
 * - D3D12:  HLSL -> DXIL
 * - Metal:  HLSL -> SPIR-V -> MSL
 */
typedef struct MZNT_ShaderCompiler
{
    rawptr handle;
} MZNT_ShaderCompiler;

/**
 * MAIN_THREAD
 * Create an instance of the shader compiler.
 * Returns zero-value, if any part of the process fails.
 */
MZNT_ShaderCompiler MZNT_CreateShaderCompiler(MZNT_ShaderCompilerConfiguration cfg, PNSLR_Allocator tempAllocator);

/**
 * MAIN_THREAD
 * Destroy the instance of the shader compiler.
 */
void MZNT_DestroyShaderCompiler(MZNT_ShaderCompiler compiler, PNSLR_Allocator tempAllocator);

EXTERN_C_END
#endif // MZNT_SHADER_COMPILER_H ===================================================
