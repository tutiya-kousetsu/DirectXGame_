#include "SceneManager.h"

void SceneManager::Update(DirectXCommon* dxCommon)
{
	//�V�[���̐؂�ւ�
	if (this->nextScene) {
		//���V�[���̏I��
		if (this->scene) {
			this->scene->Finalize();
			delete this->scene;
		}

		//�V�[���؂�ւ�
		this->scene = this->nextScene;
		this->nextScene = nullptr;

		this->scene->Initialize(dxCommon);
	}
	this->scene->Update();
}

void SceneManager::Draw(DirectXCommon* dxCommon)
{
	this->scene->Draw(dxCommon);
}
