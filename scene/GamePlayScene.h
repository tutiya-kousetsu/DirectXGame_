#pragma once

#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
#include "PostEffect.h"
#include "Input.h"
#include "Enemy.h"
#include "PlayerBullet.h"
#include "GameOver.h"
#include "GameClear.h"
#include "Floor/Floor.h"
#include "Obstacle.h"
#include "FollowingCamera.h"
#include <memory>
#include <sstream>

class Player;
/// <summary>
/// �Q�[���v���C�V�[��
/// </summary>
class GamePlayScene : public BaseScene
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
	
	//void LoadEnemyPopData();

	//void UpdataEnemyPopCommand();

	void CheckAllCollision();
private:
	
	Sprite* sprite = nullptr;
	Sprite* LifeSprite = nullptr;
	Sprite* LifeSprite2 = nullptr;
	Sprite* LifeSprite3 = nullptr;
	Sprite* spriteBG = nullptr;
	//Camera* camera = nullptr;
	// �J����
	std::unique_ptr<FollowingCamera> camera;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	PostEffect* postEffect[2] = {};

	Obstacle* obstacle = nullptr;
	//Object3d* playerObj = nullptr;
	Object3d* skyObj = nullptr;
	Model* skyModel = nullptr;

	//std::unique_ptr<PlayerBullet> playerBullet;
	Player* player = nullptr;
	//std::unique_ptr<Player> player;
	Enemy* enemy[15] = {};
	PlayerBullet* playerBullet = nullptr;
	EnemyBullet* enemyBullet = nullptr;
	Floor* floor = nullptr;
	XMFLOAT3 enePos;
	XMFLOAT3 playerPos;
	XMFLOAT2 spPos;
	std::stringstream enemyPopCommands;
	XMFLOAT3 bulPos;
	int playerLife = 3;
	int gameScore = 0;
	float flagTimer = 0;
	bool bulFlag = true;
	int flag = 0;
	bool eneFlag[15] = {};

	// �J�����֌W
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	float scaleX = 1.0f / (float)WinApp::window_width;
	float scaleY = 1.0f / (float)WinApp::window_height;
	bool viewDirty = false;
	float distance = 20.0f;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

};