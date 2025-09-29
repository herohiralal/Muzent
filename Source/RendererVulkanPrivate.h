#if defined(MZNT_IMPLEMENTATION) && MZNT_VULKAN
#include "__Prelude.h"
#include "Renderer.h"
EXTERN_C_BEGIN

typedef struct MZNT_VulkanRenderer
{
    MZNT_Renderer parent;
} MZNT_VulkanRenderer;

MZNT_VulkanRenderer* MZNT_CreateRenderer_Vulkan(MZNT_RendererConfiguration config);
b8 MZNT_DestroyRenderer_Vulkan(MZNT_VulkanRenderer* renderer);

EXTERN_C_END
#endif
