#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

#include"SpriteCommon.h"

/// <summary>
/// スプライト1枚分のデータ
/// </summary>
class Sprite
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	// 頂点データ
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};
	// 定数バッファ用データ構造体
	struct ConstBufferData {
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat;   // ３Ｄ変換行列
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">X反転するか</param>
	/// <param name="isFlipY">Y反転するか</param>
	static Sprite* Create(UINT texNumber, DirectX::XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 頂点バッファの転送
	/// </summary>
	void TransferVertexBuffer();

	/// <summary>
	/// 毎フレーム更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetPosition(const XMFLOAT2& position) { this->position = position; }

	void SetRotation(float rotation) { this->rotation = rotation; }

	void SetSize(const XMFLOAT2& size) { this->size = size; }

	void SetTexLeftTop(const XMFLOAT2& texLeftTop) { this->texLeftTop = texLeftTop; }

	void SetTexSize(const XMFLOAT2& texSize) { this->texSize = texSize; }

protected:
	//頂点バッファ;
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//定数バッファ;
	ComPtr<ID3D12Resource> constBuff;
	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	XMFLOAT2 position = { 0,0 };
	// ワールド行列
	XMMATRIX matWorld;
	// 色(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// テクスチャ番号
	UINT texNumber = 0;
	// 大きさ
	XMFLOAT2 size = { 100.0f, 100.0f };
	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0, 0 };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ左上座標
	XMFLOAT2 texLeftTop = { 0, 0 };
	// テクスチャ切り出しサイズ
	XMFLOAT2 texSize = { 100.0f, 100.0f };
	//非表示
	bool isInvisible = false;
	// 射影行列
	static XMMATRIX matProjection;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// パイプラインセット
	PipelineSet* pipelineSet = nullptr;
};

