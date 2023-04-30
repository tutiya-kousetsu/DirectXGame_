#pragma once
#include "DirectXCommon.h"

//前方宣言
class SceneManager;

/// <summary>
/// シーン基底
/// </summary>
class BaseScene
{
	
public:
	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon) = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon) = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) { this->sceneManager = sceneManager; }
protected:
	//シーンマネージャー(借りてくるもの)
	SceneManager* sceneManager = nullptr;
};

