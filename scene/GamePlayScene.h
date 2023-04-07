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
#include "obstacle/Obstacle.h"
#include "FrontEnemy.h"
#include "LeftEnemy.h"
#include "RightEnemy.h"
#include "BackEnemy.h"
#include "wall/Wall.h"
#include "door/Door.h"
#include "Player.h"
#include "TouchableObject.h"
#include "Phase.h"
#include <memory>
#include <list>
#include <sstream>

//�O���錾
class ParticleManager;
class CollisionManager;
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

	//�h�A��G�̃t�F�[�Y�ɍ��킹�ē�����
	void DoorMove();

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

	//�G�̔����f�[�^�̓ǂݍ���
	void LoadEnemyPopData();
	//�G�z�u�̃R�}���h�X�V
	void UpdataEnemyPopCommand();

	//�ǂ̔����f�[�^�̓ǂݍ���
	void LoadWallPopData();
	//�ǔz�u�̃R�}���h�X�V
	void UpdataWallPopCommand();

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
	std::unique_ptr<Sprite> sprite;
	//HP
	std::unique_ptr<Sprite> LifeSprite;
	std::unique_ptr<Sprite> LifeSprite2;
	std::unique_ptr<Sprite> LifeSprite3;
	std::unique_ptr<Sprite> LifeSprite4;
	std::unique_ptr<Sprite> LifeSprite5;
	std::unique_ptr<Sprite> LifeSprite6;
	std::unique_ptr<Sprite> LifeSprite7;
	std::unique_ptr<Sprite> LifeSprite8;
	std::unique_ptr<Sprite> alignment;
	std::unique_ptr<Sprite> damage;
	bool damageFlag = false;
	int32_t damageTime = 60;
	int phaseCount = 0;
	XMFLOAT2 phasePos;
	std::unique_ptr<Phase> phase;

	//�C�[�Y�C���pframe
	float inFrame = 0.f;
	//�C�[�Y�A�E�g�pframe
	float outFrame[8]{};

	bool frameFlag = false;
	std::unique_ptr<Sprite> spriteBG;
	// �J����
	std::unique_ptr<FollowingCamera> camera;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	PostEffect* postEffect[2] = {};

	
	//��Q��
	std::list<std::unique_ptr<Obstacle>> obstacles;
	std::stringstream obstaclePopCom;

	//�v���C���[
	std::unique_ptr<Player> player;
	int playerLife = 8;
	//�V��
	std::unique_ptr<Object3d> skyObj;

	//�G
	std::list < std::unique_ptr<FrontEnemy>> frontEnemy;
	std::list < std::unique_ptr<LeftEnemy>> leftEnemy;
	std::list < std::unique_ptr<RightEnemy>> rightEnemy;
	std::list < std::unique_ptr<BackEnemy>> backEnemy;

	std::stringstream enemyPopCom;
	std::stringstream leftPopCom;
	std::stringstream rightPopCom;
	std::stringstream backPopCom;
	//��
	std::unique_ptr<TouchableObject> floor;

	//�p�[�e�B�N��
	ParticleManager* particleMan = nullptr;
	CollisionManager* collisionMan = nullptr;
	
	//�h�A
	Door* door[8]{};
	XMFLOAT3 doorPos[8]{};
	XMFLOAT3 doorRot[8]{};

	//��
	std::list<std::unique_ptr<Wall>> walls;
	std::stringstream wallPopCom;
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
	int32_t wait = 0;
	//�t���O
	bool bulFlag = true;
	bool waitFlag = false;
	bool timeFlag = false;
	bool lifeFlag = false;
	//�t�F�[�Y
	int fEnePhase = 0;
	int fWaitPhase = 0;
	int lEnePhase = 0;
	int lWaitPhase = 0;
	int rEnePhase = 0;
	int rWaitPhase = 0;
	int bEnePhase = 0;
	int bWaitPhase = 0;
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