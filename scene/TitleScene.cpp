#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"
#include "Player.h"

void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	//�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/Title.png");
	//�X�v���C�g�̐���
	titleSp = Sprite::Create(1, { 0.0f,0.0f });
	titleSp->SetPosition({ 0,0 });
	//�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(2, L"Resources/space.png");
	spaceSp = Sprite::Create(2, { 0.0f,0.0f });
	
	//�J�����̏�����
	camera.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(camera.get());

	//���̃I�u�W�F�N�g����
	//floorModel = Model::CreateFromOBJ("FloorBox");
	floor = TouchableObject::Create(Model::CreateFromOBJ("FloorBox"));
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });

	//�f�[�^�ǂݍ���
	skyModel = Model::CreateFromOBJ("skydome");
	skyObj = Object3d::Create();
	skyObj->SetModel(skyModel);

	//���@�̃I�u�W�F�N�g�Z�b�g+������
	player = player->Create(Model::CreateFromOBJ("octopus"));

	// �J���������_���Z�b�g
	camera->SetTarget({ 0, -30, 70 });
	camera->SetEye({ 0, 15, -30 });
	camera->SetDistance(10.0f);
}

void TitleScene::Finalize()
{
	delete player;
	delete floor;
	delete skyModel;
	delete skyObj;
	//�X�v���C�g�ʉ��
	delete titleSp;
	delete spaceSp;
	delete backSp;
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	camera->Update();
	//�}�E�X�̍��N���b�N��������Ă�����
	if (input->TriggerKey(DIK_SPACE))
	{
		spacePos = spaceSp->GetPosition();
		spacePos.y += 3.0f;
		spaceSp->SetPosition(spacePos);
		//�V�[���؂�ւ�
		BaseScene* scene = new GamePlayScene();
		this->sceneManager->SetNextScene(scene);
	}
	player->Update();
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