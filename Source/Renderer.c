#define MZNT_IMPLEMENTATION
#include "RendererPrivate.h"

#include "DynamicDispatchSwitchboard/Enable.h"

MZNT_RHI_FN_TWO_ARG(
    CreateRenderer,
    (config.type),

    MZNT_Renderer*,             /* ret */      DYNA,
    MZNT_RendererConfiguration, config,        STAT,
    PNSLR_Allocator,            tempAllocator, STAT
)

MZNT_RHI_FN_ONE_ARG(
    WaitTillRendererIdle,
    (renderer ? renderer->type : 0),

    b8,       /* ret */ STAT,
    Renderer, renderer, DYNA
)

MZNT_RHI_FN_TWO_ARG(
    DestroyRenderer,
    (renderer ? renderer->type : 0),

    b8,              /* ret */      STAT,
    Renderer,        renderer,      DYNA,
    PNSLR_Allocator, tempAllocator, STAT
)

MZNT_RHI_FN_THREE_ARG(
    CreateRendererSurfaceFromWindow,
    (renderer ? renderer->type : 0),

    MZNT_RendererSurface*, /* ret */      DYNA,
    Renderer,              renderer,      DYNA,
    MZNT_WindowHandle,     windowHandle,  STAT,
    PNSLR_Allocator,       tempAllocator, STAT
)

MZNT_RHI_FN_TWO_ARG(
    DestroyRendererSurface,
    (surface ? surface->type : 0),

    b8,              /* ret */      STAT,
    RendererSurface, surface,       DYNA,
    PNSLR_Allocator, tempAllocator, STAT
)

MZNT_RHI_FN_FOUR_ARG(
    ResizeRendererSurface,
    (surface ? surface->type : 0),

    b8,              /* ret */      STAT,
    RendererSurface, surface,       DYNA,
    u16,             width,         STAT,
    u16,             height,        STAT,
    PNSLR_Allocator, tempAllocator, STAT
)

MZNT_RHI_FN_SIX_ARG(
    BeginFrame,
    (surface ? surface->type : 0),

    MZNT_RendererCommandBuffer*, /* ret */      DYNA,
    RendererSurface,             surface,       DYNA,
    f32,                         r,             STAT,
    f32,                         g,             STAT,
    f32,                         b,             STAT,
    f32,                         a,             STAT,
    PNSLR_Allocator,             tempAllocator, STAT
)

MZNT_RHI_FN_TWO_ARG(
    EndFrame,
    (surface ? surface->type : 0),

    b8,              /* ret */      STAT,
    RendererSurface, surface,       DYNA,
    PNSLR_Allocator, tempAllocator, STAT
)

MZNT_RHI_FN_FOUR_ARG(
    CreateSwapChainFromWindow,
    (renderer ? renderer->type : 0),

    MZNT_SwapChain*,             /* ret */ DYNA,
    Renderer,                    renderer, DYNA,
    MZNT_WindowHandle,           hnd,      STAT,
    MZNT_SwapChainConfiguration, cfg,      STAT,
    PNSLR_Allocator, t, STAT
)

MZNT_RHI_FN_THREE_ARG(
    ReconfigureSwapChain,
    (swapChain ? swapChain->type : 0),

    b8,                          /* ret */  STAT,
    SwapChain,                   swapChain, DYNA,
    MZNT_SwapChainConfiguration, cfg,       STAT,
    PNSLR_Allocator,             t,         STAT
)

MZNT_RHI_FN_TWO_ARG(
    DestroySwapChain,
    (swapChain ? swapChain->type : 0),

    b8,              /* ret */  STAT,
    SwapChain,       swapChain, DYNA,
    PNSLR_Allocator, t,         STAT
)

MZNT_RHI_FN_ONE_ARG(
    GetSwapChainTextureFormat,
    (swapChain ? swapChain->type : 0),

    MZNT_TextureFormat, /* ret */  STAT,
    SwapChain,          swapChain, DYNA
)

MZNT_RHI_FN_ONE_ARG(
    IterateSwapChainAndGetIndex,
    (swapChain ? swapChain->type : 0),

    u8,        /* ret */  STAT,
    SwapChain, swapChain, DYNA
)

#include "DynamicDispatchSwitchboard/Disable.h"
