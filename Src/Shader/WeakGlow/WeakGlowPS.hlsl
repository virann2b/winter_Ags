#include "../Common/VertexToPixelHeader.hlsli"
#define PS_INPUT VertexToPixelLit
#include "../Common/Pixel/PixelShader3DHeader.hlsli"
#include "../Common/Pixel/ModelPixelCommon.hlsli"

cbuffer WeakGlowParam : register(b8)
{
    float4 glowColor;   // rgb: glow color, a: strength (0-1)
};

float4 main(PS_INPUT i) : SV_TARGET
{
    
    float4 tex = diffuseMapTexture.Sample(diffuseMapSampler, i.uv);
    float4 base = tex * i.diffuse;
    float3 color = CalcModelColor(base.rgb, i.vwPos, i.normal);
    float3 toCamera = normalize(-i.vwPos);
    
    float rim = pow(1.0 - saturate(abs(dot(normalize(i.normal), toCamera))), 2.0);
    
    color += glowColor.rgb * ((rim * 1.5 + 0.25) * glowColor.a);
    
    return float4(color, base.a);

}
