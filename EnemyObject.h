#pragma once
#include "Object3d.h"

class EnemyObject : public Object3d
{
public:
	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static EnemyObject* Create(Model* model = nullptr);

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(Model* model);

public:
	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

protected:
	bool alive = true;
	std::unique_ptr<Object3d> object;
	int32_t shootTimer = 0;
	int enemyTimer = 0;
	int enemyPopFlag = 0;
	bool bulFlag = true;
	int life = 2;
	float move = 0.2f;
	float moveY = 0.2f;
	bool appFlag = true;
};

