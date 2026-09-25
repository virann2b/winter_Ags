#pragma once

// 頂点シェーダーの種類
enum class VERTEX_SHADER_TYPE
{
    // ポストエフェクト用
    PostEffect,

	// 3Dモデル用(通常)
    Default,

	// 3Dモデル用(スキニング)
    Skinning,

	// 3Dモデル用(スキニング+テクスチャアニメーション)
    Water,
};

// ピクセルシェーダーの種類
enum class PIXEL_SHADER_TYPE
{
    // トゥーン
    Toon,

    // 
    Dissolve,

    // 
    Water,


    Test,

	// グレースケールポストエフェクト
    GrayScale,

    // ブラウン管ポストエフェクト
    CRT,

    // 集中線ポストエフェクト
    FocusLines,
};