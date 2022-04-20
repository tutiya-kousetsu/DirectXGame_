#include "GamePlayScene.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"

void GamePlayScene::Initialize()
{
	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteCommon::GetInstance()->LoadTexture(1, L"Resources/gamePlay.png");
	//spriteCommon->LoadTexture(2, L"Resources/house.png");

	//�X�v���C�g�̐���
	sprite = Sprite::Create(1, { 0,0 }, false, false);
	sprite->SetPosition({ 0,0,0 });

	//OBJ���烂�f���f�[�^��ǂݍ���
	modelPost = Model::LoadFromObj("post");

	//3D�I�u�W�F�N�g����
	objPost = Object3d::Create();
	objPost->SetModel(modelPost);

	//�����ǂݍ���
	//Audio::GetInstance()->SoundLoadWave("Alarm01.wav");

	//�����Đ�
	//audio->SoundPlayWave("Alarm01.wav", true);

	//objPost->SetPosition({ -10,0,-5 });
	//objChr->SetPosition({ +10,0,+5 });
	//�J�����̏�����
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(camera);

	camera->SetEye({ 0, 0, -50 });
	camera->SetTarget({ 0,0,-10 });

	//���f�������w�肵�ăt�@�C����ǂݍ���
	FbxLoader::GetInstance()->LoadModelFromFile("cube");

}

void GamePlayScene::Finalize()
{
	delete camera;
	//�X�v���C�g�ʉ��
	delete sprite;
	//3D���f�����
	delete modelPost;
	//3D�I�u�W�F�N�g���
	delete objPost;
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();

	XMFLOAT3 playerPos = objPost->GetPosition();

	if (input->PushKey(DIK_UP))
	{
		if (playerPos.x <= 70) {
			playerPos.x += speed * 2;
		}
		if (playerPos.x >= 70) {
			playerPos.x = -70;
		}
	}

	// ���W����
	/*if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{

	}*/

	objPost->SetPosition(playerPos);

	//X���W,Y���W���w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//�X�V
	objPost->Update();
	//objChr->Update();
	camera->Update();
	sprite->Update();
}

void GamePlayScene::Draw()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
	//�X�v���C�g�`��
	sprite->Draw();

	//3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw();

	//3D�I�u�W�F�N�g�̕`��
	objPost->Draw();

	//3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();

	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
}