#include "HelloTriangle.common.hlsl"

[shader("vertex")]
VertexOutput main(uint vid: SV_VertexID)
{
    VertexOutput output;
    output.posCS = float4(positions[vid], 0.0, 1.0);
    __target_switch
    {
    case spirv:
        output.posCS.y *= -1;
        break;
    default:
        break;
    }
    output.vtCol = colours[vid];
    return output;
}
