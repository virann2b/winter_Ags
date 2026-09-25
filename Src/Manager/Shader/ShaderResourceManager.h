#pragma once

#include <map>
#include <string>

#include "ShaderTypeDefine.h"

class ShaderResourceManager
{
private:

	static ShaderResourceManager* ins;

	ShaderResourceManager() :
		vertexShaderList(),
		pixelShaderList()
	{
	}
	~ShaderResourceManager() = default;

public:

	static void CreateIns(void) { if (ins == nullptr)ins = new ShaderResourceManager(); }
	static ShaderResourceManager& GetIns(void) { return *ins; }
	static void DeleteIns(void) {
		if (ins != nullptr) { 
			ins->ReleaseAllShader();
			delete ins; 
			ins = nullptr;
		}
	}


	// 頂点シェーダーの生成
	void CreateVertexShader(VERTEX_SHADER_TYPE type);

	// 頂点シェーダーの取得
	int GetVertexShader(VERTEX_SHADER_TYPE type);

	// 頂点シェーダーの解放
	void ReleaseVertexShader(VERTEX_SHADER_TYPE type);

	// 頂点シェーダーを全て解放する
	void ReleaseAllVertexShader(void);


	// ピクセルシェーダーの生成
	void CreatePixelShader(PIXEL_SHADER_TYPE type);

	// ピクセルシェーダーの取得
	int GetPixelShader(PIXEL_SHADER_TYPE type);

	// ピクセルシェーダーの解放
	void ReleasePixelShader(PIXEL_SHADER_TYPE type);

	// ピクセルシェーダーを全て解放する
	void ReleaseAllPixelShader(void);


	// 全てのシェーダーの解放
	void ReleaseAllShader(void);

private:

	// 頂点シェーダーのパステーブル
	const std::map<VERTEX_SHADER_TYPE, std::string> VERTEX_SHADER_PATH_TABLE = {

	{
		VERTEX_SHADER_TYPE::PostEffect,
		"Data/Shader/PostEffect/PostEffectVS.cso"
	},

	{
		VERTEX_SHADER_TYPE::Default,
		"Data/Shader/Model/DefaultVS.cso"
	},

	{
		VERTEX_SHADER_TYPE::Skinning,
		"Data/Shader/Model/DefaultVS.cso"
	},

	};

	// 頂点シェーダー
	std::map<VERTEX_SHADER_TYPE, int> vertexShaderList;


	// ピクセルシェーダーのパステーブル
	const std::map<PIXEL_SHADER_TYPE, std::string> PIXEL_SHADER_PATH_TABLE = {

		// =========================
		// Model
		// =========================

		{
			PIXEL_SHADER_TYPE::Toon,
			"Data/Shader/Model/ToonPS.cso"
		},

		{
			PIXEL_SHADER_TYPE::Dissolve,
			"Data/Shader/Model/DissolvePS.cso"
		},

		{
			PIXEL_SHADER_TYPE::Water,
			"Data/Shader/Model/WaterPS.cso"
		},

		{
			PIXEL_SHADER_TYPE::Test,
			"Data/Shader/Model/TestPS.cso"
		},

		// =========================
		// PostEffect
		// =========================

		{
			PIXEL_SHADER_TYPE::GrayScale,
			"Data/Shader/PostEffect/GrayScalePS.cso"
		},

		{
			PIXEL_SHADER_TYPE::CRT,
			"Data/Shader/PostEffect/CRT_PS.cso"
		},

		{
			PIXEL_SHADER_TYPE::FocusLines,
			"Data/Shader/PostEffect/FocusLinesPS.cso"
		},

	};

	// ピクセルシェーダー
	std::map<PIXEL_SHADER_TYPE, int> pixelShaderList;
};