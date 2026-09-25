#include "../Common/PostEffectCommon.hlsli"

cbuffer VignetteParam : register(b0)
{
    float intensity;
    float radius;
    float softness;
    float padding;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = GetUV(input);
    float4 color = GetSceneColor(uv);
    float2 p = uv - 0.5f;
    p.x *= SCREEN_SIZE.x / SCREEN_SIZE.y;
    float mask = 1.0f - smoothstep(radius, radius + max(softness, 0.0001f), length(p));
    color.rgb *= lerp(1.0f, mask, saturate(intensity));
    return color;
}
