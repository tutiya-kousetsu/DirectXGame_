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
#include "Obstacle.h"
#include "Floor/Floor.h"
#include "FrontEnemy.h"
#include "LeftEnemy.h"
#include "RightEnemy.h"
#include "BackEnemy.h"
#include "Wall.h"
#include "Door.h"
#include <memory>
#include <list>
#include <sstream>

//�O���錾
class Player;
class Line;
class ParticleManager;
class CollisionManager;
class TouchableObject;
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


	//�O�G�̓����蔻��
	void FrontColl();
	//���G�̓����蔻��
	void LeftColl();
	//�E�G�̓����蔻��
	void RightColl();
	//���G�̓����蔻��
	void BackColl();

	void CheckAllCollision();
private:
	//�X�v���C�g
	Sprite* sprite = nullptr;
	Sprite* LifeSprite = nullptr;
	Sprite* LifeSprite2 = nullptr;
	Sprite* LifeSprite3 = nullptr;
	Sprite* LifeSprite4 = nullptr;
	Sprite* LifeSprite5 = nullptr;
	Sprite* LifeSprite6 = nullptr;
	Sprite* LifeSprite7 = nullptr;
	Sprite* LifeSprite8 = nullptr;
	Sprite* spriteBG = nullptr;
	// �J����
	std::unique_ptr<FollowingCamera> camera;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	PostEffect* postEffect[2] = {};

	
	//��Q��
	std::list<std::unique_ptr<Obstacle>> obstacles;
	std::stringstream obstaclePopCom;

	//�v���C���[
	Player* player = nullptr;
	PlayerBullet* playerBullet = nullptr;
	int playerLife = 8;
	//�V��
	Object3d* skyObj = nullptr;
	Model* skyModel = nullptr;
	Model* obModel = nullptr;
	
	//�G
	FrontEnemy* frontEnemy[11]{};
	LeftEnemy* leftEnemy[7]{};
	RightEnemy* rightEnemy[4]{};
	BackEnemy* backEnemy[2]{};
	EnemyBullet* enemyBullet = nullptr;
	//��
	TouchableObject* floor = nullptr;
	Model* floorModel = nullptr;
	//�p�[�e�B�N��
	ParticleManager* particleMan = nullptr;
	CollisionManager* collisionMan = nullptr;
	

	//�|�W�V����
	XMFLOAT3 playerPos;
	XMFLOAT3 enePos;
	XMFLOAT3 bulPos;
	//�X�v���C�g
	XMFLOAT2 spPos;
	//�G�̃|�W�V����
	XMFLOAT3 frontEnePos[11];
	XMFLOAT3 leftEnePos[7];
	XMFLOAT3 rightEnePos[4];
	XMFLOAT3 backEnePos[2];

	//�^�C�}�[
	int waitTimer = 0;
	//�t���O�֌W
	bool bulFlag = true;
	bool waitFlag = false;
	bool lifeFlag = false;
	int fEneFlag = 0;
	int lEneFlag = 0;
	int rEneFlag = 0;
	int bEneFlag = 0;
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