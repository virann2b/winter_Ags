#pragma once

#include "ShaderBase.h"

#include "../../../pch.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Shader/ShaderResourceManager.h"

/// <summary>
/// 弱点を突かれた時に属性の色で光らせるシェーダー
/// 光の強さが0の間はシェーダーを使わず、通常の描画のまま
/// </summary>
class WeakGlowShader : public ShaderBase
{
public:

	/// <param name="model">光らせるモデル（頂点タイプの判定に使う）</param>
	WeakGlowShader(int model) :
		ShaderBase(),
		model(model),
		constantBufferHandle(-1),
		param{},
		isUsable_(false)
	{
	}
	~WeakGlowShader() override = default;

	void Init(void) override
	{
		//モデルの頂点タイプから使う頂点シェーダーを決める
		VERTEX_SHADER_TYPE vsType = VERTEX_SHADER_TYPE::Default1Frame;
		isUsable_ = true;
		switch (MV1GetTriangleListVertexType(model, 0))
		{
		case DX_MV1_VERTEX_TYPE_1FRAME:			vsType = VERTEX_SHADER_TYPE::Default1Frame; break;
		case DX_MV1_VERTEX_TYPE_4FRAME:			vsType = VERTEX_SHADER_TYPE::Default4Frame; break;
		case DX_MV1_VERTEX_TYPE_8FRAME:			vsType = VERTEX_SHADER_TYPE::Default8Frame; break;
		case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:	vsType = VERTEX_SHADER_TYPE::DefaultNMap1Frame; break;
		case DX_MV1_VERTEX_TYPE_NMAP_4FRAME:	vsType = VERTEX_SHADER_TYPE::DefaultNMap4Frame; break;
		case DX_MV1_VERTEX_TYPE_NMAP_8FRAME:	vsType = VERTEX_SHADER_TYPE::DefaultNMap8Frame; break;
		//対応していない頂点タイプは光らせない（通常描画のまま）
		default: isUsable_ = false; break;
		}
		if (!isUsable_) { return; }

		ShaderResourceManager& manager = ShaderResourceManager::GetIns();

		manager.CreateVertexShader(vsType);
		manager.CreatePixelShader(PIXEL_SHADER_TYPE::WeakGlow);

		vertexShaderHandle = manager.GetVertexShader(vsType);
		pixelShaderHandle = manager.GetPixelShader(PIXEL_SHADER_TYPE::WeakGlow);

		//シェーダーが読み込めなかった場合（.csoが無いなど）は光らせない
		if (vertexShaderHandle == -1 || pixelShaderHandle == -1) { isUsable_ = false; return; }

		constantBufferHandle = CreateShaderConstantBuffer(sizeof(PARAM));
	}

	/// <summary>光の色と強さを設定</summary>
	/// <param name="color">光の色（0～1）</param>
	/// <param name="strength">光の強さ（0で光らない）</param>
	void SetGlow(const Vector3& color, float strength)
	{
		param.r = color.x;
		param.g = color.y;
		param.b = color.z;
		param.strength = strength;
	}

	void Apply(void) override
	{
		//光っていない間は通常の描画のまま
		if (!IsGlowing()) { return; }

		//パラメータをシェーダーへ送る
		PARAM* buffer = static_cast<PARAM*>(GetBufferShaderConstantBuffer(constantBufferHandle));
		*buffer = param;
		UpdateShaderConstantBuffer(constantBufferHandle);
		SetShaderConstantBuffer(constantBufferHandle, DX_SHADERTYPE_PIXEL, CONSTANT_BUFFER_SLOT);

		ShaderBase::Apply();
	}

	void ResetApply(void) override
	{
		if (!IsGlowing()) { return; }

		SetShaderConstantBuffer(-1, DX_SHADERTYPE_PIXEL, CONSTANT_BUFFER_SLOT);

		ShaderBase::ResetApply();
	}

	void Release(void) override
	{
		if (constantBufferHandle != -1)
		{
			DeleteShaderConstantBuffer(constantBufferHandle);
			constantBufferHandle = -1;
		}

		ShaderBase::Release();
	}

private:

	//シェーダー側の register(b8) に合わせる
	static constexpr int CONSTANT_BUFFER_SLOT = 8;

	//シェーダーへ送るパラメータ（float4 の並びに合わせる）
	struct PARAM
	{
		float r, g, b;
		float strength;
	};

	//光らせるモデル
	int model;

	//定数バッファ
	int constantBufferHandle;

	PARAM param;

	//このモデルで使えるか
	bool isUsable_;

	//光っているか
	bool IsGlowing(void) const { return isUsable_ && param.strength > 0.0f; }
};
