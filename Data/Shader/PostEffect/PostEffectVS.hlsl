struct VS_INPUT
{
    float4 pos : POSITION;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    const float2 screenSize = float2(1280.0f, 720.0f);

    float2 clipPos;
    clipPos.x = input.pos.x / screenSize.x * 2.0f - 1.0f;
    clipPos.y = 1.0f - input.pos.y / screenSize.y * 2.0f;

    output.pos = float4(clipPos, 0.0f, 1.0f);
    return output;
}
