#pragma once
#include "Sprite.h"
#include "PipelineSet.h"
//#include "SpriteCommon.h"

class PostEffect :
	public Sprite
{
protected://エイリアス
//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreateGraphicsPipelineState(const wchar_t* psName);

public:
	//テクスバッファ
	ComPtr<ID3D12Resource> texBuff[2];
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTVデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSVデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;

private:
	static const float clearColor[4];

};

