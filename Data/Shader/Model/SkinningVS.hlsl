cbuffer CameraBuffer : register(b0)
{
    row_major float4x4 viewProjection;
};

cbuffer SkinningBuffer : register(b1)
{
    row_major float4x4 boneMatrices[128];
};

struct VS_INPUT
{
    float3 pos : POSITION0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;

    uint4 boneIndex : BLENDINDICES0;
    float4 boneWeight : BLENDWEIGHT0;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float2 uv : TEXCOORD2;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // -------------------------
    // スキニング行列を作成
    // -------------------------

    float4x4 skinMatrix =
        boneMatrices[input.boneIndex.x] * input.boneWeight.x +
        boneMatrices[input.boneIndex.y] * input.boneWeight.y +
        boneMatrices[input.boneIndex.z] * input.boneWeight.z +
        boneMatrices[input.boneIndex.w] * input.boneWeight.w;


    // -------------------------
    // 頂点座標をスキニング
    // -------------------------

    float4 skinnedPos =
        mul(
            float4(input.pos, 1.0f),
            skinMatrix
        );


    // -------------------------
    // 法線もスキニング
    // -------------------------

    float3 skinnedNormal =
        mul(
            float4(input.normal, 0.0f),
            skinMatrix
        ).xyz;

    skinnedNormal =
        normalize(skinnedNormal);


    // -------------------------
    // 射影変換
    // -------------------------

    output.pos =
        mul(
            skinnedPos,
            viewProjection
        );


    // PixelShaderへ渡す
    output.worldPos =
        skinnedPos.xyz;

    output.normal =
        skinnedNormal;

    output.uv =
        input.uv;


    return output;
}