Texture2D MainTexture : register(t0);
Texture2D NoiseTexture : register(t1);
SamplerState MainSampler : register(s0);
SamplerState NoiseSampler : register(s1);

cbuffer DissolveParam : register(b0)
{
    float dissolveRate;
    float edgeWidth;
    float2 padding;
    float3 edgeColor;
    float padding2;
};

struct PS_INPUT
{
    float4 pos      : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 normal   : TEXCOORD1;
    float2 uv       : TEXCOORD2;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 baseColor = MainTexture.Sample(MainSampler, input.uv);
    float noise = NoiseTexture.Sample(NoiseSampler, input.uv).r;
    clip(noise - dissolveRate);
    float edge = 1.0f - smoothstep(dissolveRate, dissolveRate + edgeWidth, noise);
    baseColor.rgb = lerp(baseColor.rgb, edgeColor, edge);
    return baseColor;
}
