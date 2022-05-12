#pragma once

#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
/// <summary>
/// �Q�[���v���C�V�[��
/// </summary>
class GamePlayScene
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon);

	void Finalize();

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

private:
	
	Sprite* sprite = nullptr;
	Model* modelPost = nullptr;
	Object3d* objPost = nullptr;
	DebugCamera* camera = nullptr;
	DirectXCommon* dxCommon = nullptr;

	Fbx_Model* fbxmodel1 = nullptr;
	Fbx_Object3d* fbxobject1 = nullptr;
};

