#pragma once

#include"PipelineSet.h"

#include <DirectXMath.h>

/// <summary>
/// スプライト共通
/// </summary>

class SpriteCommon
{
public:
	static SpriteCommon* GetInstance();

	// テクスチャの最大枚数
	static const int spriteSRVCount = 512;

/// <summary>
/// 初期化
/// </summary>
	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture(UINT texnumber, const wchar_t* filename);

	/// <summary>
	/// ルートデスクリプタテーブルの設定
	/// </summary>
	/// <param name="rootParameterIndex">ルートパラメータ番号</param>
	/// <param name="texNumber">テクスチャ番号</param>
	void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texNumber);

	/// <summary>
	/// テクスチャ取得
	/// </summary>
	/// <param name="texnumber">番号</param>
	/// <returns></returns>
	ID3D12Resource* GetTexBuff(int texNumber);

	ID3D12Device* GetDevice() { return device; }

	const DirectX::XMMATRIX& GetMatProjection() { return matProjection; }

	ID3D12GraphicsCommandList* GetCommandList() {return cmdList;}

protected:
	// パイプラインセット
	PipelineSet pipelineSet;
	// 射影行列
	DirectX::XMMATRIX matProjection{};
	// テクスチャ用デスクリプタヒープの生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャリソース（テクスチャバッファ）の配列
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
	//デバイス(借りてくる)
	ID3D12Device* device = nullptr;

	ID3D12GraphicsCommandList* cmdList = nullptr;

protected:
	// パイプライン生成
	void CreateGraphicsPipeline();

};

