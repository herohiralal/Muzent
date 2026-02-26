float4 ApplyVertexShaderOutputCorrection(float4 col)
{
    float4 result = col;
    __target_switch
    {
    case spirv:
        break;
    case hlsl:
        result.y *= -1;
        break;
    }

    return result;
}

static float2 positions[3] = float2[](
    float2(0.0, -0.5),
    float2(0.5, 0.5),
    float2(-0.5, 0.5));

static float3 colours[3] = float3[](
    float3(0.0, 1.0, 1.0),
    float3(1.0, 0.0, 1.0),
    float3(1.0, 1.0, 0.0));

struct VertexOutput
{
    float4 posCS : SV_Position;
    float3 vtCol : TEXCOORD0;
};

[shader("vertex")]
VertexOutput vertMain(uint vid: SV_VertexID)
{
    VertexOutput output;
    output.posCS = float4(positions[vid], 0.0, 1.0);
    output.posCS = ApplyVertexShaderOutputCorrection(float4(positions[vid], 0.0, 1.0));
    output.vtCol = colours[vid];
    return output;
}

[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target
{
    float3 colour = inVert.vtCol;
    return float4(colour, 1.0);
}
