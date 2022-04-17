#pragma once

#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"

/// <summary>
/// �Q�[���v���C�V�[��
/// </summary>
class GamePlayScene
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	void Finalize();

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	Sprite* sprite = nullptr;
	Model* modelPost = nullptr;
	Object3d* objPost = nullptr;
	DebugCamera* camera = nullptr;
};

