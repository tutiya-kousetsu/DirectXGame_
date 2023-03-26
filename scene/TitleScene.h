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

class TitleScene : public BaseScene
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

private:

	std::unique_ptr <Sprite> titleSp;
	std::unique_ptr <Sprite> spaceSp;
	std::unique_ptr <Sprite> backSp;
	Input* input = nullptr;
	std::unique_ptr<TouchableObject> floor;
	//�V��
	std::unique_ptr <Object3d> skyObj;
	std::unique_ptr<DebugCamera> camera;
	XMFLOAT3 cameraPos;
	std::unique_ptr<Player> player;
};

