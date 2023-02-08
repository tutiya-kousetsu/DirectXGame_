#pragma once
#include "Object3d.h"
class Player;

class EnemyObject : public Object3d
{
public:
	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static EnemyObject* Create(Model* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model* model);

	void SetPlayer(Player* player) { this->player = player; }

protected:
	Player* player = nullptr;
};

