#include "../Common/PostEffectCommon.hlsli"

cbuffer PixelateParam : register(b0)
{
    float pixelSize;
    float intensity;
    float2 padding;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = GetUV(input);
    float px = max(pixelSize, 1.0f);
    float2 pixelCount = SCREEN_SIZE / px;
    float2 pixelUV = (floor(uv * pixelCount) + 0.5f) / pixelCount;
    return lerp(GetSceneColor(uv), GetSceneColor(pixelUV), saturate(intensity));
}
