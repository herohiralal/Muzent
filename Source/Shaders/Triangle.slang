static float2 positions[3] = float2[](
    float2(0.0, -0.5),
    float2(0.5, 0.5),
    float2(-0.5, 0.5));

static float3 colors[3] = float3[](
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
    output.vtCol = colors[vid];
    return output;
}

[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target
{
    float3 color = inVert.vtCol;
    return float4(color, 1.0);
}
