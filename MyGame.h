#pragma once
#include <vector>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "Audio.h"
#include "Object3d.h"
#include "Model.h"
#include "Sprite.h"

/// <summary>
/// �Q�[���ŗL�̃N���X
/// </summary>
class MyGame
{

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	bool IsEndRequst() { return endRequst; }

private:
	bool endRequst = false;
	//�|�C���^�u����
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	DebugCamera* camera = nullptr;
	SpriteCommon* spriteCommon = nullptr;

	std::vector<Sprite*> sprites;
	Model* modelPost = nullptr;
	Object3d* objPost = nullptr;
};

