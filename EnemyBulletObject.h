#pragma once
#include "Object3d.h"
class EnemyBulletObject : public Object3d
{
public:
	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static EnemyBulletObject* Create(Model* model = nullptr);

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(Model* model);

protected:
	bool alive = true;
	std::unique_ptr<Object3d> object;

};

