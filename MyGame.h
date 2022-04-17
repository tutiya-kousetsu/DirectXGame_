#pragma once
#include <vector>

#include "Framework.h"
#include "Sprite.h"

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

	std::vector<Sprite*> sprites;
	Model* modelPost = nullptr;
	Object3d* objPost = nullptr;
};

