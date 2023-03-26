#pragma once
#include "Object3d.h"
#include "PlayerBullet.h"
#include "GameObject.h"
#include <memory>
#include <list>
#include <forward_list>
class ParticleManager;

using namespace DirectX;

class Player : public Object3d
{
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

	//�^�C�g���p�X�V
	void TitleUpdate();

	//�ړ�
	void move(float speed = 0.3f);

	//�W�����v
	void jump();

	//�`��
	void Draw() override;

	//�U��
	bool Shoot();

	//�����蔻��
	void OnCollision(const CollisionInfo& info) override;

	//�p�[�e�B�N����
	void CreateParticle();
	//�e���X�g�擾
	//const std::unique_ptr<PlayerBullet>& GetBullet() { return bullet; }
	const std::list < std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets; }
	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

	inline bool GetAlive() const { return alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

	inline int GetPhase() { return operatePhase; }

public:
	int operatePhase = 0;
	bool moveFlag = false;
	bool jumpOpFlag = false;
	bool shotFlag = false;
private:
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	bool bulFlag = true;
	// �}�E�X
	POINT mousePos{};
	ParticleManager* particleMan = nullptr;

	//�U��(���������ǂ����̃t���O�A�^�C�}�[�A��)
	int atPower = 1;
	int atTimer = 0;
	bool atFlag = false;
	int mWaiteCount = 0;
	bool canShot = false;
	// �W�����v
	bool alive = true;
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
	DirectX::XMVECTOR fallV;
};

