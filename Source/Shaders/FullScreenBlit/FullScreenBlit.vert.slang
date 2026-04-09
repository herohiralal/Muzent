#include "FullScreenBlit.common.hlsl"

[shader("vertex")]
VSOutput main(uint vertexID: SV_VertexID)
{
    VSOutput o;
    o.uv = float2((vertexID << 1) & 2, vertexID & 2);
    o.position = float4(o.uv * float2(2.0, -2.0) + float2(-1.0, 1.0), 0.0, 1.0);
    __target_switch
    {
    case spirv:
        o.position.y *= -1;
        break;
    default:
        break;
    }
    return o;
}
