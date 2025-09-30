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
 * Represents an opaque handle to the application instance.
 * Matches Dvaarpaal's app handle.
 * - On Windows, this is an HINSTANCE.
 * - On OSX, this is an NSApplication*.
 * - On Android, this is a struct android_app*.
 */
typedef struct MZNT_AppHandle
{
    u64 handle;
} MZNT_AppHandle;

/**
 * Represents an opaque handle to a window.
 * Matches Dvaarpaal's window handle.
 * - On Windows, this is an HWND.
 * - On OSX, this is an NSWindow*.
 * - On Android, this is an ANativeWindow*.
 */
typedef struct MZNT_WindowHandle
{
    u64 handle;
} MZNT_WindowHandle;

/**
 * Configuration structure for renderer creation.
 */
typedef struct MZNT_RendererConfiguration
{
    MZNT_RendererType type;
    MZNT_AppHandle    appHandle;
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
    MZNT_AppHandle    appHandle;
} MZNT_Renderer;

/**
 * Creates a renderer instance based on the provided configuration.
 */
MZNT_Renderer* MZNT_CreateRenderer(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator);

/**
 * Destroys the given renderer instance and frees associated resources.
 */
b8 MZNT_DestroyRenderer(MZNT_Renderer* renderer, PNSLR_Allocator tempAllocator);

/**
 * A surface that can be drawn on top of.
 */
typedef struct MZNT_RendererSurface
{
    MZNT_RendererType type;
} MZNT_RendererSurface;

/**
 * Creates a rendering surface for the given window and application handles.
 */
MZNT_RendererSurface* MZNT_CreateRendererSurface(MZNT_Renderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator);

/**
 * Destroys the given rendering surface and frees associated resources.
 */
b8 MZNT_DestroyRendererSurface(MZNT_RendererSurface* surface, PNSLR_Allocator tempAllocator);

/**
 * Handles resizing the given rendering surface to the specified dimensions.
 */
b8 MZNT_ResizeRendererSurface(MZNT_RendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator);

typedef struct MZNT_RendererCommandBuffer
{
    MZNT_RendererType type;
} MZNT_RendererCommandBuffer;

/**
 * Creates a command buffer for the given renderer.
 */
MZNT_RendererCommandBuffer* MZNT_CreateRendererCommandBuffer(MZNT_Renderer* renderer, PNSLR_Allocator tempAllocator);

/**
 * Destroys the given command buffer and frees associated resources.
 */
b8 MZNT_DestroyRendererCommandBuffer(MZNT_RendererCommandBuffer* commandBuffer, PNSLR_Allocator tempAllocator);

EXTERN_C_END
#endif // MZNT_RENDERER_H ==========================================================
