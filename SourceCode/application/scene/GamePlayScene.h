#pragma once

#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
#include "PostEffect.h"
#include "PlayerBullet.h"
#include "Tutorial.h"
#include "TitleScene.h"
#include "FollowingCamera.h"
#include "Input.h"
#include "Obstacle.h"
#include "FrontEnemy.h"
#include "LeftEnemy.h"
#include "RightEnemy.h"
#include "BackEnemy.h"
#include "Wall.h"
#include "Door.h"
#include "Player.h"
#include "TouchableObject.h"
#include "Phase.h"
#include "PlayerLife.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Shake.h"
#include <memory>
#include <list>
#include <sstream>

//�O���錾
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
	std::unique_ptr<Sprite> onAlignment;
	std::unique_ptr<Sprite> damage;
	std::unique_ptr<Sprite> clear;
	//�\������Ă���Ԃ̕b��
	int32_t damageTime = 20;
	int phaseCount = 0;
	std::unique_ptr<Phase> phase;

	//�C�[�Y�C���pframe
	float inFrame = 0.f;
	float easFrame = 0.f;
	
	std::unique_ptr<Sprite> spriteBG;
	// �J����
	std::unique_ptr<FollowingCamera> camera;
	std::unique_ptr<DebugCamera> debugCam;
	Camera* nowCamera = nullptr;

	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	std::unique_ptr<PostEffect> postEffect;
	
	//�Q�[�����I��������ɈÂ������̔��a
	float endEfRadius = 1000;
	
	//��Q��
	std::list<std::unique_ptr<Obstacle>> obstacles;
	std::stringstream obstaclePopCom;
	//�v���C���[
	std::unique_ptr<Player> player;
	int life = 0;
	std::unique_ptr<PlayerLife> playerLife;
	//�V��
	std::unique_ptr<Object3d> skyObj;
	std::unique_ptr<Object3d> object;
	//�G
	std::list < std::unique_ptr<FrontEnemy>> frontEnemy;
	std::list < std::unique_ptr<LeftEnemy>> leftEnemy;
	std::list < std::unique_ptr<RightEnemy>> rightEnemy;
	std::list < std::unique_ptr<BackEnemy>> backEnemy;

	std::stringstream enemyPopCom;
	//��
	std::unique_ptr<TouchableObject> floor;

	//�p�[�e�B�N��
	ParticleManager* particleMan = nullptr;
	CollisionManager* collisionMan = nullptr;
	
	BaseCollider* collider = nullptr;

	//�h�A
	std::unique_ptr<Door> door;
	//��
	std::list<std::unique_ptr<Wall>> walls;
	std::stringstream wallPopCom;
	//�X�v���C�g
	XMFLOAT2 phasePos;
	XMFLOAT2 clearPos;
	XMFLOAT2 spPos;
	//�|�W�V����
	//�h�A
	XMFLOAT3 doorPos;
	//���@
	XMFLOAT3 playerPos;
	//���@�̃X�P�[��
	XMFLOAT3 playerScale;
	//���[�e�[�V����
	XMFLOAT3 doorRot;
	XMFLOAT3 playerRot;
	//�X�v���C�g�̃J���[
	XMFLOAT4 damageColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT4 color;

	//�^�C�}�[
	int32_t wait = 0;
	int32_t clearTime = 0;
	int32_t landTime = 0;
	int32_t sceneTime = 0;
	int32_t numbTime = 0;
	int32_t downTime = 0;
	//�t���O
	bool bulFlag = true;//�e�̃t���O
	bool waitFlag = false;//�҂��Ă�Ƃ��̃t���O
	bool clearFlag = false;
	bool clearTFlag = false;
	bool damageFlag1 = false;//�G�e�ɓ���������
	bool damageFlag2 = false;//�G�e�ɓ���������
	bool damageFlag3 = false;//�G�e�ɓ���������
	bool damageFlag4 = false;//�G�e�ɓ���������
	bool rayFlag = false;
	bool endFlag = false;//�Q�[�����I�������
	bool landFlag = false;//���n���Ă��邩
	bool numbFlag = false;
	bool hpFlag = false;
	bool phaseFlag = false;
	bool upFlag = false;
	//�t�F�[�Y
	//�O�̓G
	int fEnePhase = 0;
	int fWaitPhase = 0;
	//���̓G
	int lEnePhase = 0;
	int lWaitPhase = 0;
	//�E�̓G
	int rEnePhase = 0;
	int rWaitPhase = 0;
	//���̓G
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