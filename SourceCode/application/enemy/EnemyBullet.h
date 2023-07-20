#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class EnemyBullet : public GameObject
{
public:
	using GameObject::GameObject;

public:
	EnemyBullet();

	~EnemyBullet();

	void Initialize(XMFLOAT3 pos, const XMVECTOR& vel);

	void Update();

	//�`��
	void Draw();

	//�Փˎ��ɌĂяo�����֐�
	void OnCollision();
public:

	//�e�������鎞��
	UINT disappearTime = 60 * 5;

	UINT frameNum = 0;
	UINT power = 1;
private:
	XMFLOAT3 position;
	XMVECTOR velocity;
	XMFLOAT4 color;
};

