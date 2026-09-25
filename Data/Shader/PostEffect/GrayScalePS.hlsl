#include "../Common/PostEffectCommon.hlsli"

cbuffer GrayScaleParam : register(b0)
{
    float intensity;
    float3 padding;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = GetSceneColor_Input(input);
    float gray = dot(color.rgb, float3(0.299f, 0.587f, 0.114f));
    color.rgb = lerp(color.rgb, gray.xxx, saturate(intensity));
    return color;
}
