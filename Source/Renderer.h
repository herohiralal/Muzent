#ifndef MZNT_RENDERER_H // =========================================================
#define MZNT_RENDERER_H
#include "__Prelude.h"
EXTERN_C_BEGIN

/**
 * Defines the available Renderer types.
 */
ENUM_START(MZNT_RendererType, u8)
    #define MZNT_RendererType_Null      ((MZNT_RendererType) 0)
    #define MZNT_RendererType_Vulkan    ((MZNT_RendererType) 1)
    #define MZNT_RendererType_DirectX12 ((MZNT_RendererType) 2)
    #define MZNT_RendererType_Metal     ((MZNT_RendererType) 3)
ENUM_END

/**
 * Configuration structure for renderer creation.
 */
typedef struct MZNT_RendererConfiguration
{
    MZNT_RendererType type;
    PNSLR_Allocator   allocator;
    utf8str           appName;
} MZNT_RendererConfiguration;

/**
 * The main renderer interface.
 * Treat as base class for specific renderer implementations.
 */
typedef struct MZNT_Renderer
{
    MZNT_RendererType type;
    PNSLR_Allocator   allocator;
} MZNT_Renderer;

/**
 * Creates a renderer instance based on the provided configuration.
 */
MZNT_Renderer* MZNT_CreateRenderer(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator);

/**
 * Destroys the given renderer instance and frees associated resources.
 */
b8 MZNT_DestroyRenderer(MZNT_Renderer* renderer, PNSLR_Allocator tempAllocator);

EXTERN_C_END
#endif // MZNT_RENDERER_H ==========================================================
