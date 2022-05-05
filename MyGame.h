#pragma once
#include <vector>

#include "BaseScene.h"
#include "Framework.h"

class GamePlayScene;

/// <summary>
/// �Q�[���ŗL�̃N���X
/// </summary>
class MyGame : public Framework
{

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

private:
	//�V�[��
	GamePlayScene* scene = nullptr;
};

