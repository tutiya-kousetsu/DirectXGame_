#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MeshCollider.h"


void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	//�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/Title.png");
	//�X�v���C�g�̐���
	titleSp.reset(Sprite::Create(1, { 0.0f,400.0f }));

	//�J�����̏�����
	camera.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(camera.get());

	//���̃I�u�W�F�N�g����
	floor.reset(TouchableObject::Create(Model::CreateFromOBJ("FloorBox")));
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });

	//�f�[�^�ǂݍ���
	skyObj.reset(Object3d::Create());
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));

	//�X�^�[�g�����̃I�u�W�F�N�g
	startObj.reset(Object3d::Create());
	startObj->SetModel(Model::CreateFromOBJ("letter"));
	startObj->SetPosition({ 13.f, 8.5f, 30.f });
	startObj->SetRotation({ 90,0,0 });
	startObj->SetScale({ 4.0f, 4.0f, 4.0f });

	//�X�^�[�g�����̃I�u�W�F�N�g
	endObj.reset(Object3d::Create());
	endObj->SetModel(Model::CreateFromOBJ("endLetter"));
	endObj->SetPosition({ -13.f, 12.5f, 30.f });
	endObj->SetRotation({ 90,0,0 });
	endObj->SetScale({ 4.0f, 4.0f, 4.0f });

	//���@�̃I�u�W�F�N�g�Z�b�g+������
	player.reset(Player::Create(Model::CreateFromOBJ("octopus")));

	startEnemy.reset(new FrontEnemy());
	startEnemy->Initialize({ 13, 6, 30 });
	startEnemy->SetPlayer(player.get());

	endEnemy.reset(new FrontEnemy());
	endEnemy->Initialize({ -13, 6, 30 });
	endEnemy->SetPlayer(player.get());

	frame.reset(new Framework());
	//frame->Initialize();
	// �J���������_���Z�b�g
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	//�v���C���[��x���̈ʒu�ɍ��킹�ăJ������������
	camera->SetTarget({ player->GetPosition().x, -20, 70 });
	camera->SetEye({ player->GetPosition().x, 10, -30 });
	camera->SetDistance(10.0f);

	camera->Update();
	startEnemy->TitleUpdate();
	endEnemy->TitleUpdate();
	player->Update();
	floor->Update();
	skyObj->Update();
	startObj->Update();
	endObj->Update();
	CheckCollision();
}

void TitleScene::CheckCollision()
{
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();

#pragma region ���e�ƓG�̓����蔻��
	//�G��|������Q�[���X�^�[�g
	Sphere pBullet;
	for (auto& pb : playerBullets) {
		if (pb->GetAlive()) {
			pBullet.center = XMLoadFloat3(&pb->GetPosition());
			pBullet.radius = pb->GetScale().x;

			//�O�̓G
			if (startEnemy) {
				if (startEnemy->GetAlive()) {
					Sphere fEnemyShape;
					fEnemyShape.center = XMLoadFloat3(&startEnemy->GetPosition());
					fEnemyShape.radius = startEnemy->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, fEnemyShape)) {
						pb->OnCollision();
						startEnemy->OnCollision();
						if (!startEnemy->GetAlive()) {
							BaseScene* scene = new GamePlayScene();
							this->sceneManager->SetNextScene(scene);
						}
					}
				}
			}
			if (endEnemy) {
				if (endEnemy->GetAlive()) {
					Sphere fEnemyShape;
					fEnemyShape.center = XMLoadFloat3(&endEnemy->GetPosition());
					fEnemyShape.radius = endEnemy->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, fEnemyShape)) {
						pb->OnCollision();
						endEnemy->OnCollision();
						if (!endEnemy->GetAlive()) {
							if (!frame->IsEndRequst()) {
								//frame->IsEndRequst() = true;
							}
						}
					}
				}
			}
		}
	}
}


void TitleScene::Draw(DirectXCommon* dxCommon)
{
	//�`��O����
	dxCommon->PreDraw();
	//�X�v���C�g�`��
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCmdList());
	//�w�i�X�v���C�g�`��

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer(dxCommon->GetCmdList());
#pragma endregion

#pragma endregion

	//3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw();
	floor->Draw();
	skyObj->Draw();
	startObj->Draw();
	endObj->Draw();
	player->Draw();
	startEnemy->Draw();
	endEnemy->Draw();
	Object3d::PostDraw();

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>
	titleSp->Draw();
	// �X�v���C�g�`��㏈��

	Sprite::PostDraw();

	//�`��㏈��
	dxCommon->PostDraw();

}