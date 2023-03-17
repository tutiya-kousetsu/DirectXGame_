#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
#include "Input.h"
#include "TouchableObject.h"
#include "Camera.h"
#include "FollowingCamera.h"
#include "DebugCamera.h"

class Player;

class TitleScene : public BaseScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

private:

	Sprite* titleSp = nullptr;
	Sprite* spaceSp = nullptr;
	Sprite* backSp = nullptr;
	DirectX::XMFLOAT2 spacePos;
	Input* input = nullptr;
	TouchableObject* floor = nullptr;
	//天球
	Object3d* skyObj = nullptr;
	Model* skyModel = nullptr;
	std::unique_ptr<DebugCamera> camera;
	DirectX::XMFLOAT3 cameraPos;
	Player* player = nullptr;
};

