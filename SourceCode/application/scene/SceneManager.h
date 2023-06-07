#pragma once
#include "BaseScene.h"
#include "Audio.h"
/// <summary>
/// シーン管理
/// </summary>
class SceneManager
{
public:
	~SceneManager();
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
};

