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
#include "FrontEnemy.h"
#include "Player.h"
#include "Framework.h"
#include "Material.h"

//前方宣言
class ParticleManager;

class Tutorial : public BaseScene
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

	void CheckAllCollision();

private:

	std::unique_ptr <Sprite> sprite;
	std::unique_ptr <Sprite> spaceUI;
	std::unique_ptr <Sprite> mouseUI;
	std::unique_ptr <Sprite> wasdUI;
	std::unique_ptr <Sprite> shotUI;
	Input* input = nullptr;
	std::unique_ptr<TouchableObject> floor;
	//天球
	std::unique_ptr <Object3d> skyObj;
	std::unique_ptr<Framework> frame;
	std::unique_ptr<FollowingCamera> camera;
	//パーティクル
	ParticleManager* particleMan = nullptr;

	XMFLOAT3 cameraPos;
	std::unique_ptr<Player> player;
	std::unique_ptr<FrontEnemy> enemy;
	std::unique_ptr<Object3d> sceneMoveObj;
	std::unique_ptr<Object3d> arrowObj;
	bool zoneFlag = false;
	bool zonePop = false;
	float rotateTime = 120;
	float rotateFlag = false;
	int operatePhase = 0;
	XMFLOAT3 playerRot;
	// カメラ関係
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	float scaleX = 0.35f / (float)WinApp::window_width;
	float scaleY = 0.35f / (float)WinApp::window_height;
	bool viewDirty = false;
	float distance = 1.0f;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
};

