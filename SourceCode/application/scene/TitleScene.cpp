#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"
#include "Tutorial.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MeshCollider.h"


void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	//�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/Title.png");
	//�X�v���C�g�̐���
	titleSp.reset(Sprite::Create(1, { 0,0 }));

	//�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(2, L"Resources/space.png");
	//�X�v���C�g�̐���
	spaceSp.reset(Sprite::Create(2, { 0,0 }));

	//�J�����̏�����
	camera.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(camera.get());

	//���̃I�u�W�F�N�g����
	floor.reset(TouchableObject::Create(Model::CreateFromOBJ("FloorBox")));
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });

	//�f�[�^�ǂݍ���
	skyObj = Object3d::Create();
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));

	//���@�̃I�u�W�F�N�g�Z�b�g+������
	player.reset(Player::Create(Model::CreateFromOBJ("octopus")));
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	//�t���O��false�̎��J������player���S�ɂ��ĉ�
	angle += XMConvertToRadians(0.6f);
	cameraPos = camera->GetEye();
	cameraPos.x = 50 * sinf(angle);
	cameraPos.y = 30;
	cameraPos.z = 50 * cosf(angle);
	//�v���C���[��x���̈ʒu�ɍ��킹�ăJ������������
	camera->SetTarget(player->GetPosition());
	camera->SetEye(cameraPos);
	camera->SetDistance(10.0f);
	if (input->TriggerKey(DIK_SPACE))//�X�y�[�X�L�[��������Ă�����
	{
		//�V�[���؂�ւ�
		BaseScene* scene = new Tutorial();
		this->sceneManager->SetNextScene(scene);
	}

	camera->Update();
	player->StopUpdate();
	player->SetRotation({ 0,180,0 });
	floor->Update();
	skyObj->Update();
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
	player->Draw();
	Object3d::PostDraw();

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>
	titleSp->Draw();
	spaceSp->Draw();
	// �X�v���C�g�`��㏈��

	Sprite::PostDraw();

	//�`��㏈��
	dxCommon->PostDraw();

}