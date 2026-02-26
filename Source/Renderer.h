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
 * Represents a mesh that can be drawn.
 */
typedef struct MZNT_Mesh
{
    MZNT_RendererType type;
} MZNT_Mesh;

/**
 * The vertex attributes that can be used in a mesh.
 */
ENUM_START(MZNT_VertexAttributeType, u8)
    #define MZNT_VertexAttributeType_Position  ((MZNT_VertexAttributeType) 0)
    #define MZNT_VertexAttributeType_Color     ((MZNT_VertexAttributeType) 1)
    #define MZNT_VertexAttributeType_Normal    ((MZNT_VertexAttributeType) 2)
    #define MZNT_VertexAttributeType_Tangent   ((MZNT_VertexAttributeType) 3)
    #define MZNT_VertexAttributeType_TexCoord0 ((MZNT_VertexAttributeType) 4)
    #define MZNT_VertexAttributeType_TexCoord1 ((MZNT_VertexAttributeType) 5)
    #define MZNT_VertexAttributeType_TexCoord2 ((MZNT_VertexAttributeType) 6)
    #define MZNT_VertexAttributeType_TexCoord3 ((MZNT_VertexAttributeType) 7)
ENUM_END

/**
 * The vertex attribute description for a mesh, specifying the type and number of components.
 */
typedef struct MZNT_VertexAttribute
{
    MZNT_VertexAttributeType type;
    u8                       componentCount;
} MZNT_VertexAttribute;

PNSLR_DECLARE_ARRAY_SLICE(MZNT_VertexAttribute);

/**
 * The types of indices that can be used in a mesh.
 * Note that 32-bit indices aren't always supported on certain platforms.
 */
ENUM_START(MZNT_IndexType, u8)
    #define MZNT_IndexType_U16 ((MZNT_IndexType) 0)
    #define MZNT_IndexType_U32 ((MZNT_IndexType) 1)
ENUM_END

/**
 * The information required to create a mesh, including vertex attributes, vertex/index data, and counts.
 * The vertex attributes must be provided in the same order as the vertex data layout.
 */
typedef struct MZNT_MeshCreateInfo
{
    PNSLR_ArraySlice(MZNT_VertexAttribute) vertexAttributes;
    MZNT_IndexType                         indexType;

    void* vertexData;
    i64   vertexCount;

    void* indexData;
    i64   indexCount;
} MZNT_MeshCreateInfo;

/**
 * Uploads a mesh to the GPU based on the provided creation information, returning a handle to the uploaded mesh.
 */
MZNT_Mesh* MZNT_UploadMesh(MZNT_Renderer* renderer, MZNT_MeshCreateInfo* createInfo, PNSLR_Allocator tempAllocator);

/**
 * Destroys the given mesh and frees associated resources.
 */
b8 MZNT_DestroyMesh(MZNT_Mesh* mesh, PNSLR_Allocator tempAllocator);

EXTERN_C_END
#endif // MZNT_RENDERER_H ==========================================================
