#include "FullScreenBlit.common.hlsl"

[shader("fragment")]
float4 main(VSOutput i)
    : SV_Target
{
    return G_SrcTexture.Sample(G_SrcTextureSampler, i.uv);
}
