#pragma once
#include "Object3d.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "GameObject.h"
#include "Shake.h"
#include <memory>
#include <list>
#include <forward_list>
class ParticleManager;

using namespace DirectX;

class Player : public Object3d
{
public:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Player* Create(Model* model = nullptr);
public:
	//�R���X�g���N�^
	//Player();

	~Player();

	bool Initialize() override;

	//�X�V
	void Update() override;

	//�����������Ȃ����p�X�V
	void StopUpdate();


	//�ړ�
	void move(float speed = 0.3f);

	//�W�����v
	void jump();

	//�X�P�[��������������
	void ScaleSmall();

	void ScaleLarge();

	//�`��
	void Draw() override;

	//�`���[�g���A���p�X�V
	void TutorialUpdate();

	//�`���[�g���A���p�`��
	void TutorialDraw(bool flag);
	//�U��
	bool Shoot();

	void Numb(bool flag = false);

	//�����蔻��
	void OnCollision(int i);

	//�p�[�e�B�N����
	void CreateParticle();
	//�e���X�g�擾
	//const std::unique_ptr<PlayerBullet>& GetBullet() { return bullet; }
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets; }
	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

	inline bool GetAlive() const { return alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }


	inline void SetLife(int life) { this->life = life; }
	inline const int GetLife() const { return life; }

	inline int GetPhase() { return operatePhase; }
	
public:
	int operatePhase = 0;
	bool moveFlag = false;
	bool jumpOpFlag = false;
	bool shotFlag = false;
private:
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	std::unique_ptr<EnemyBullet> eb;
	bool bulFlag = true;
	// �}�E�X
	POINT mousePos{};
	ParticleManager* particleMan = nullptr;
	int32_t shootTimer = 0;

	//�U��(���������ǂ����̃t���O�A�^�C�}�[�A��)
	int atPower = 1;
	int atTimer = 0;
	bool atFlag = false;
	int mWaiteCount = 0;
	bool canShot = false;
	// �W�����v
	bool alive = true;
	int life = 8;
	bool jumpFlag = false;
	bool secondFlag = false;
	int jumpCount = 2;
	float g = 0.25;//�d��
	float jumpSpeed = 0;
	float speed = 7.0f;	//���x
	float t = 0.01f;		//����
	float speed2 = 2.55f;	//���x
	//�ڒn�t���O
	bool onGround = true;
	//�����x�N�g��
	XMVECTOR fallV;
	//�V�F�C�N�p
	std::unique_ptr<Shake> shake = nullptr;
	bool shakeF = false;
	XMFLOAT3 shakePos = { 0.0f,0.0f,0.0f };
};

