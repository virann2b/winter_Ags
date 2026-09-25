#include "ToonShader.h"

#include "../../../Manager/Shader/ShaderResourceManager.h"

#include "../../../pch.h"

ToonShader::ToonShader() : ShaderBase()
{
}

void ToonShader::Init(void)
{
    // 必要なシェーダーを生成（基本的には既に生成済み）
    ShaderResourceManager::GetIns().CreateVertexShader(VERTEX_SHADER_TYPE::Skinning);
    ShaderResourceManager::GetIns().CreatePixelShader(PIXEL_SHADER_TYPE::Toon);

    // ハンドルを取得
    vertexShaderHandle = ShaderResourceManager::GetIns().GetVertexShader(VERTEX_SHADER_TYPE::Skinning);
    pixelShaderHandle = ShaderResourceManager::GetIns().GetPixelShader(PIXEL_SHADER_TYPE::Toon);
}