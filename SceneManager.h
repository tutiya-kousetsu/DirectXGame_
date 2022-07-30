#pragma once

#include "BaseScene.h"

/// <summary>
/// �V�[���Ǘ�
/// </summary>
class SceneManager
{
public:
	static SceneManager* GetInstance();
	
	/// <summary>
	/// �X�V
	/// </summary>
	void Update(DirectXCommon* dxCommon);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	//���V�[���\��
	void SetNextScene(BaseScene* nextScene) { this->nextScene = nextScene; }
private:
	//���̃V�[��
	BaseScene* scene = nullptr;
	//���̃V�[��
	BaseScene* nextScene = nullptr;

	//�V���O���g���p�^�[��(�B��)
	SceneManager() = default;
	~SceneManager();
	SceneManager(const SceneManager&) = delete;
	void operator = (const SceneManager&) = delete;
};

