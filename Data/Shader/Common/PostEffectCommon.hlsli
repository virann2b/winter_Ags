Texture2D MainTexture : register(t0);
SamplerState MainSampler : register(s0);

static const float2 SCREEN_SIZE = float2(1440.0f, 810.0f);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
};

// PS_INPUTからUVを取得
float2 GetUV(PS_INPUT input)
{
    return input.pos.xy / SCREEN_SIZE;
}

// UVからシーンカラーを取得
float4 GetSceneColor(float2 uv)
{
    return MainTexture.Sample(MainSampler, uv);
}

// PS_INPUTから直接シーンカラーを取得
float4 GetSceneColor_Input(PS_INPUT input)
{
    return GetSceneColor(GetUV(input));
}

// 疑似乱数
float Rand(float2 p)
{
    return frac(
        sin(dot(p, float2(12.9898f, 78.233f))) *
        43758.5453f
    );
}