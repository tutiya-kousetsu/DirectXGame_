#pragma once
#include "Object3d.h"
class Player;

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

	void SetPlayer(Player* player) { this->player = player; }

protected:
	Player* player = nullptr;
};

