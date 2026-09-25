#include "../Common/PostEffectCommon.hlsli"

cbuffer FocusLinesParam : register(b0)
{
    float time;
    float intensity;
    float speed;
    float lineCount;
};

float Random(float seed)
{
    return frac(sin(seed * 12.9898f) * 43758.5453f);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = GetUV(input);
    float4 color = GetSceneColor(uv);
    float2 p = uv - 0.5f;
    float aspect = SCREEN_SIZE.x / SCREEN_SIZE.y;
    p.x *= aspect;

    float dist = length(p);
    float2 dir = normalize(p + 0.000001f);
    float edgeX = abs(dir.x) > 0.0001f ? (aspect * 0.5f) / abs(dir.x) : 99999.0f;
    float edgeY = abs(dir.y) > 0.0001f ? 0.5f / abs(dir.y) : 99999.0f;
    float radial01 = saturate(dist / min(edgeX, edgeY));

    const float PI = 3.14159265f;
    float angle01 = atan2(p.y, p.x) / (2.0f * PI) + 0.5f;
    float lineValue = angle01 * max(lineCount, 1.0f);
    float lineIndex = floor(lineValue);
    float localLine = frac(lineValue);

    float anim = time * speed;
    float frameA = floor(anim);
    float frameB = frameA + 1.0f;
    float blendRate = smoothstep(0.0f, 1.0f, frac(anim));

    float seedA = lineIndex + frameA * 97.0f;
    float seedB = lineIndex + frameB * 97.0f;
    float randomLength = lerp(Random(seedA + 11.0f), Random(seedB + 11.0f), blendRate);
    float randomWidth = lerp(Random(seedA + 23.0f), Random(seedB + 23.0f), blendRate);
    float randomBrightness = lerp(Random(seedA + 37.0f), Random(seedB + 37.0f), blendRate);
    float randomOffset = lerp(Random(seedA + 53.0f), Random(seedB + 53.0f), blendRate);

    float lineCenter = 0.5f + (randomOffset - 0.5f) * 0.45f;
    float lineDistance = abs(localLine - lineCenter);
    float innerRadius = lerp(0.65f, 0.90f, randomLength);
    float innerMask = smoothstep(innerRadius, innerRadius + 0.06f, radial01);
    float innerWidth = lerp(0.0015f, 0.005f, randomWidth);
    float outerWidth = lerp(0.055f, 0.18f, randomWidth);
    float lineWidth = lerp(innerWidth, outerWidth, pow(radial01, 2.2f));
    float lineMask = 1.0f - smoothstep(lineWidth * 0.45f, lineWidth, lineDistance);
    float brightness = lerp(0.25f, 1.0f, randomBrightness);
    float edgeStrength = smoothstep(0.25f, 1.0f, radial01);

    color.rgb += lineMask * innerMask * edgeStrength * brightness * intensity;
    return color;
}
