#include "../Common/VertexToPixelHeader.hlsli"
#define PS_INPUT VertexToPixelLit
#include "../Common/Pixel/PixelShader3DHeader.hlsli"
#include "../Common/Pixel/ModelPixelCommon.hlsli"

cbuffer WeakGlowParam : register(b4)
{
    float4 glowColor;   // rgb: glow color, a: strength (0-1)
};

float4 main(PS_INPUT i) : SV_TARGET
{
    float4 tex = diffuseMapTexture.Sample(diffuseMapSampler, i.uv);
    float4 base = tex * i.diffuse;
    float3 color = CalcModelColor(base.rgb, i.vwPos, i.normal);

    // Rim light: stronger toward the silhouette
    float3 toCamera = normalize(-i.vwPos);
    float rim = 1.0 - saturate(abs(dot(normalize(i.normal), toCamera)));
    rim = pow(rim, 2.0);

    // Tint the whole body slightly, and glow strongly on the outline
    float glow = (rim * 1.5 + 0.25) * glowColor.a;
    color += glowColor.rgb * glow;

    color = FinalizeModelColor(color, i.vwPos);
    return float4(color, base.a);
}
