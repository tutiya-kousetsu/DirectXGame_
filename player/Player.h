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

	bool Initialize() override;

	//�X�V
	void Update() override;

	//�ړ�
	void move(float speed = 0.3f);

	//�W�����v
	void jump();

	//��]
	void playerRot();

	//�`��
	void Draw() override;

	//�U��
	void Shoot();

	void OnCollision(const CollisionInfo& info) override;

	void FloorCollision();

	//�e���X�g�擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets; }

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

private:
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	bool bulFlag = true;
	// �}�E�X
	POINT mousePos{};
	ParticleManager* particleMan = nullptr;
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

};

