#pragma once

#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
#include "PostEffect.h"
#include "PlayerBullet.h"
#include "GameOver.h"
#include "GameClear.h"
#include "FollowingCamera.h"
#include "Input.h"
#include "Enemy.h"
#include "Obstacle.h"
#include "Floor/Floor.h"
#include "FrontEnemy.h"
#include "LeftEnemy.h"
#include "RightEnemy.h"
#include "BackEnemy.h"
#include <memory>
#include <sstream>

//�O���錾
class Player;
class Line;
class ParticleManager;

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
	
	/// <summary>
	/// ��Q���̔����f�[�^�̓ǂݍ���
	/// </summary>
	void LoadObstaclePopData();

	/// <summary>
	/// ��Q���z�u�̃R�}���h�X�V
	/// </summary>
	void UpdataObstaclePopCommand();

	void CheckAllCollision();
private:
	
	Sprite* sprite = nullptr;
	Sprite* LifeSprite = nullptr;
	Sprite* LifeSprite2 = nullptr;
	Sprite* LifeSprite3 = nullptr;
	Sprite* spriteBG = nullptr;
	// �J����
	std::unique_ptr<FollowingCamera> camera;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	PostEffect* postEffect[2] = {};

	Obstacle* obstacle[4] = {};
	std::stringstream obstaclePopCom;
	XMFLOAT3 obstaclePos[4] = {};
	Object3d* skyObj = nullptr;
	Model* skyModel = nullptr;

	//std::unique_ptr<PlayerBullet> playerBullet;
	Player* player = nullptr;
	//std::unique_ptr<Player> player;
	Enemy* enemy = nullptr;
	FrontEnemy* frontEnemy = nullptr;
	LeftEnemy* leftEnemy = nullptr;
	RightEnemy* rightEnemy = nullptr;
	BackEnemy* backEnemy = nullptr;
	//std::list<std::unique_ptr<Enemy>> enemy;
	PlayerBullet* playerBullet = nullptr;
	EnemyBullet* enemyBullet = nullptr;
	Floor* floor = nullptr;
	ParticleManager* particleMan = nullptr;
	//Line* line = nullptr;
	XMFLOAT3 enePos;

	XMFLOAT3 playerPos;
	XMFLOAT2 spPos;
	std::stringstream enemyPopCommands;
	XMFLOAT3 bulPos;
	int playerLife = 3;
	int enemyLife = 3;

	int gameScore = 0;
	float flagTimer = 0;
	bool bulFlag = true;
	int flag = 0;
	bool eneFlag = false;
	bool posFlag = false;
	// �J�����֌W
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	float scaleX = 0.35f / (float)WinApp::window_width;
	float scaleY = 0.35f / (float)WinApp::window_height;
	bool viewDirty = false;
	float distance = 1.0f;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

};