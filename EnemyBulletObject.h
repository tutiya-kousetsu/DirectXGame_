#pragma once
#include "Object3d.h"
class EnemyBulletObject : public Object3d
{
public:
	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static EnemyBulletObject* Create(Model* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model* model);
public:
	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

protected:
	bool alive = true;
	std::unique_ptr<Object3d> object;

};

