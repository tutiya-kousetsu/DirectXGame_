#pragma once
#include "Sprite.h"

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

public:
	//デバイス(借りてくる)
	ID3D12Device* device = nullptr;
	// パイプラインセット
	PipelineSet pipelineSet;
	ComPtr<ID3D12Resource> texBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTVデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSVデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
};

