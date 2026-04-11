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
 * Defines the available texture formats.
 */
ENUM_START(MZNT_TextureFormat, u8)
    #define MZNT_TextureFormat_Unknown            ((MZNT_TextureFormat) 0)
    #define MZNT_TextureFormat_D32_Float          ((MZNT_TextureFormat) 1)
    #define MZNT_TextureFormat_B8G8R8A8_UNorm     ((MZNT_TextureFormat) 2)
    #define MZNT_TextureFormat_R8G8B8A8_UNorm     ((MZNT_TextureFormat) 3)
    #define MZNT_TextureFormat_R16G16B16A16_UNorm ((MZNT_TextureFormat) 4)
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
 * MAIN_THREAD
 * Creates a renderer instance based on the provided configuration.
 */
MZNT_Renderer* MZNT_CreateRenderer(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator);

/**
 * MAIN_THREAD
 * Destroys the given renderer instance and frees associated resources.
 */
b8 MZNT_DestroyRenderer(MZNT_Renderer* renderer, PNSLR_Allocator tempAllocator);

/**
 * MAIN_THREAD
 * Halts the thread until all the work that's already submitted to the renderer is done.
 * Return value is insignificant.
 *
 * Warning! - it won't necessarily account for work that gets submitted after this on other threads.
 */
b8 MZNT_WaitTillRendererIdle(MZNT_Renderer* renderer);

//+skipreflect

/**
 * The number of frames that can be processed concurrently.
 */
#define MZNT_NUM_FRAMES_IN_FLIGHT 2

//-skipreflect

/**
 * A command buffer for recording rendering commands.
 */
typedef struct MZNT_RendererCommandBuffer
{
    MZNT_RendererType type;
} MZNT_RendererCommandBuffer;

/**
 * A surface that can be drawn on top of.
 */
typedef struct MZNT_RendererSurface
{
    MZNT_RendererType type;
} MZNT_RendererSurface;

/**
 * Creates a rendering surface for the given window and application handles.
 * Uses 3 frames in flight.
 */
MZNT_RendererSurface* MZNT_CreateRendererSurfaceFromWindow(MZNT_Renderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator);

/**
 * Destroys the given rendering surface and frees associated resources.
 */
b8 MZNT_DestroyRendererSurface(MZNT_RendererSurface* surface, PNSLR_Allocator tempAllocator);

/**
 * Handles resizing the given rendering surface to the specified dimensions.
 */
b8 MZNT_ResizeRendererSurface(MZNT_RendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator);

/**
 * Begins a new frame for the given rendering surface, returning a command buffer for recording commands.
 * The returned command buffer must be ended with MZNT_EndFrame (to submit drawing for the current frame).
 */
MZNT_RendererCommandBuffer* MZNT_BeginFrame(MZNT_RendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator);

/**
 * Ends the current frame for the given rendering surface, submitting recorded commands for execution.
 */
b8 MZNT_EndFrame(MZNT_RendererSurface* surface, PNSLR_Allocator tempAllocator);

/**
 * Configuration structure for swap-chain.
 */
typedef struct MZNT_SwapChainConfiguration
{
    u16 width;
    u16 height;
    b8  vSync;
    u8  framesInFlight;
} MZNT_SwapChainConfiguration;

/**
 * A swap-chain corresponding to a window that can be rendered to.
 */
typedef struct MZNT_SwapChain
{
    MZNT_RendererType type;
} MZNT_SwapChain;

/**
 * MAIN_THREAD
 * Create a swap-chain for the given window, with the given renderer, for a custom number of frames in flight.
 * If width/height are uninitialised, they will be derived from the window.
 */
MZNT_SwapChain* MZNT_CreateSwapChainFromWindow(MZNT_Renderer* renderer, MZNT_WindowHandle windowHandle, MZNT_SwapChainConfiguration cfg, PNSLR_Allocator tempAllocator);

/**
 * MAIN_THREAD
 * Reconfigure a swap-chain with new properties.
 * All configuration values must be initialised appropriately.
 */
b8 MZNT_ReconfigureSwapChain(MZNT_SwapChain* swapChain, MZNT_SwapChainConfiguration cfg, PNSLR_Allocator tempAllocator);

/**
 * MAIN_THREAD
 * Destroy the swap-chain, freeing up associated resources.
 */
b8 MZNT_DestroySwapChain(MZNT_SwapChain* swapChain, PNSLR_Allocator tempAllocator);

/**
 * MAIN_THREAD
 * Get the texture format of the swap-chain.
 */
MZNT_TextureFormat MZNT_GetSwapChainTextureFormat(MZNT_SwapChain* swapChain);

/**
 * MAIN_THREAD
 * Acquire the next image in the swap-chain.
 * Returns 255 to indicate that this operation was not valid.
 * A common example is if the window is minimised.
 */
u8 MZNT_IterateSwapChainAndGetIndex(MZNT_SwapChain* swapChain);

EXTERN_C_END
#endif // MZNT_RENDERER_H ==========================================================
