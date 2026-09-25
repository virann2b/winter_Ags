Texture2D MainTexture : register(t0);
SamplerState MainSampler : register(s0);

cbuffer WaterParam : register(b0)
{
    float time;
    float waveStrength;
    float waveScale;
    float alpha;
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
    float2 uv = input.uv;
    uv.x += sin(uv.y * waveScale + time) * waveStrength;
    uv.y += cos(uv.x * waveScale - time * 0.8f) * waveStrength;
    float4 color = MainTexture.Sample(MainSampler, uv);
    color.a *= alpha;
    return color;
}
