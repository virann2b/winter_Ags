//cbuffer TestShaderParam : register(b10)
//{
//    float time;
//    float scale;
//    float intensity;
//    float padding;
//};

//struct PS_INPUT
//{
//    float4 pos : SV_POSITION;
//};

//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float2 cell = floor(input.pos.xy / max(scale, 1.0f));

//    float checker = fmod(cell.x + cell.y, 2.0f);

//    float pulse = sin(time * 4.0f) * 0.5f + 0.5f;

//    float3 colorA = float3(1.0f, 0.1f, 0.2f);

//    float3 colorB = float3(0.1f, 0.8f, 1.0f);

//    float3 color = lerp(colorA, colorB, checker);

//    color *= lerp(0.4f, 1.5f, pulse * intensity);

//    return float4(saturate(color), 1.0f);
//}
float4 main() : SV_TARGET
{
    return float4(
        1.0f,
        0.0f,
        1.0f,
        1.0f
    );
}