#pragma once
#include "Fbx_Model.h"
#include "Camera.h"
//#include "FbxLoader.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
class Fbx_Object3d
{
protected://エイリアス
	//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス
//定数バッファ用データ構造体
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;	//ビュープロジェクション行列
		XMMATRIX world;		//ワールド行列
		XMFLOAT3 cameraPos;	//カメラ座標
	};

public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

protected://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;

public://静的メンバ関数
	//setter
	static void SetDevice(ID3D12Device* device) { Fbx_Object3d::device = device; }
	static void SetCamera(Camera* camera) { Fbx_Object3d::camera = camera; }

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Fbx_Model* fbx_model) { this->fbx_model = fbx_model; }

private://静的メンバ変数
	// コマンドリスト
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	//ローカルスケール
	XMFLOAT3 scale = { 1, 1, 1 };
	//x, y, z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0, 0, 0 };
	//ローカル座標
	XMFLOAT3 position = { 0, 0, 0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;
	//モデル
	Fbx_Model* fbx_model = nullptr;
};

