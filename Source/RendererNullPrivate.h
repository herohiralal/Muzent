#if defined(MZNT_IMPLEMENTATION)
#include "__Prelude.h"
#include "Renderer.h"
EXTERN_C_BEGIN

typedef struct MZNT_NullRenderer
{
    MZNT_Renderer parent;
} MZNT_NullRenderer;

MZNT_NullRenderer* MZNT_CreateRenderer_Null(MZNT_RendererConfiguration config);
b8 MZNT_DestroyRenderer_Null(MZNT_NullRenderer* renderer);

EXTERN_C_END
#endif
