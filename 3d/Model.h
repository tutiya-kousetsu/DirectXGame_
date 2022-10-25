#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

/// <summary>
/// 3Dモデル
/// </summary>
class Model
{
public:
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv;  // uv座標
	};

	//定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		DirectX::XMFLOAT3 ambient;
		float pad1;
		DirectX::XMFLOAT3 diffuse;
		float pad2;
		DirectX::XMFLOAT3 specular;
		float alpha;
	};

	//マテリアル
	struct Material
	{
		std::string name;//マテリアル名
		DirectX::XMFLOAT3 ambient;//アンビエント影響度
		DirectX::XMFLOAT3 diffuse;//ディヒューズ影響度
		DirectX::XMFLOAT3 specular;//スペキュラ影響度
		float alpha;//
		std::string textureFilename;//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

public:
	/// <summary>
	/// OBJファイルからDモデルを読み込む
	/// </summary>
	/// <param name="modelname">モデル名</param>
	/// <returns>モデル</returns>
	static Model* LoadFromObj(const std::string& modelname);

	/// <summary>
	/// マテリアルの読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="rootParamIndexMaterial">マテリアルのルートパラメータ番号指定</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);

	static void SetDevice(ID3D12Device* device) { Model::device = device; }

private: // メンバ関数
	//デバイス(借りてくる)
	static ID3D12Device* device;

	// 頂点データ配列
	std::vector<VertexPosNormalUv>vertices;
	// 頂点インデックス配列
	std::vector<unsigned short>indices;

	//マテリアル
	Material material;

	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	//マテリアル用定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;

private:
	void LoadFromOBJInternal(const std::string& modelname);

	// デスクリプタヒープの初期化
	void InitializeDescriptorHeap();

	//各種バッファーの生成
	void CreateBuffers();
};

