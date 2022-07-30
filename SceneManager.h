#pragma once

#include "BaseScene.h"

/// <summary>
/// シーン管理
/// </summary>
class SceneManager
{
public:
	static SceneManager* GetInstance();
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update(DirectXCommon* dxCommon);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	//次シーン予約
	void SetNextScene(BaseScene* nextScene) { this->nextScene = nextScene; }
private:
	//今のシーン
	BaseScene* scene = nullptr;
	//次のシーン
	BaseScene* nextScene = nullptr;

	//シングルトンパターン(隠蔽)
	SceneManager() = default;
	~SceneManager();
	SceneManager(const SceneManager&) = delete;
	void operator = (const SceneManager&) = delete;
};

