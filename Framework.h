#pragma once

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "Audio.h"
#include "Object3d.h"


/// <summary>
/// �t���[�����[�N
/// </summary>
class Framework
{
public:
	/// <summary>
	/// ���s
	/// </summary>
	void Run();
	
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;

	virtual bool IsEndRequst() { return endRequst; }

protected:
	bool endRequst = false;
	//�|�C���^�u����
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	DebugCamera* camera = nullptr;
	SpriteCommon* spriteCommon = nullptr;
};

