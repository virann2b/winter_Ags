Texture2D MainTexture : register(t0);
SamplerState MainSampler : register(s0);

cbuffer ToonParam : register(b0)
{
    float3 lightDirection;
    float shadowThreshold;
    float3 lightColor;
    float shadowStrength;
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
    float4 tex = MainTexture.Sample(MainSampler, input.uv);
    float ndl = saturate(dot(normalize(input.normal), -normalize(lightDirection)));
    float toon = step(shadowThreshold, ndl);
    float lighting = lerp(shadowStrength, 1.0f, toon);
    return float4(tex.rgb * lightColor * lighting, tex.a);
}
