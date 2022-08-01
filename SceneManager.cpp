#include "SceneManager.h"

SceneManager::~SceneManager()
{
	//最後のシーンの終了と解放
	this->scene->Finalize();
	delete this->scene;
}

void SceneManager::Update(DirectXCommon* dxCommon)
{
	//シーンの切り替え
	if (this->nextScene) {
		//旧シーンの終了
		if (this->scene) {
			this->scene->Finalize();
			delete this->scene;
		}

		//シーン切り替え
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
