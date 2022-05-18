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
	objPost2 = Object3d::Create();
	objPost2->SetModel(modelPost);
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

	//playerPos = objPost->GetPosition();
	playerPos = { -70, 0, 0 };
	//playerPos2 = objPost2->GetPosition();
	playerPos2 = { 70, 0, 0 };

}

void GamePlayScene::Finalize()
{
	delete objPost2;
	//�J�����̉��
	delete camera;
	//�X�v���C�g���
	delete sprite;
	//3D���f�����
	delete modelPost;
	//3D�I�u�W�F�N�g���
	delete objPost;

}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();
	objPost->SetScale({ 1.5f, 1.5f, 1.5f });
	objPost2->SetScale({ 2.0f, 2.0f, 2.0f });
	//�������^��
		/*if (playerPos.x <= 70) {
			playerPos.x += speed;
			playerPos.y += speed2;
			speed2 -= t;
		}
		if (playerPos.x >= 70 || playerPos.y <= -40) {
			playerPos.x = -70;
			playerPos.y = -10;
			t = 0.01;
			speed = 0.75f;
			speed2 = 0.75f;
		}*/

	if (playerPos.x <= 70) {
		playerPos.x += speed;

	}
	if (playerPos.x >= 70) {
		playerPos.x = -70;
		speed = 0.75f;
	}
	if (playerPos2.x >= -70) {
		playerPos2.x -= speed2;

	}
	if (playerPos2.x <= -70) {
		playerPos2.x = 70;
		speed = 0.75f;
	}

	//	float dx = abs(playerPos.x - playerPos2.x);

	float enemyi;
	enemyi = sqrtf((playerPos.x - playerPos2.x) * (playerPos.x - playerPos2.x) + (playerPos.y - playerPos2.y) * (playerPos.y - playerPos2.y));
	if (R + R >= enemyi) {
		speed = -speed;
		speed2 = -speed2;
	}

	objPost->SetPosition(playerPos);
	objPost2->SetPosition(playerPos2);
	//X���W,Y���W���w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//�X�V
	objPost->Update();
	objPost2->Update();
	//objChr->Update();
	camera->Update();
	sprite->Update();
}

void GamePlayScene::Draw()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
	//�X�v���C�g�`��
	//sprite->Draw();

	//3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw();

	//3D�I�u�W�F�N�g�̕`��
	objPost->Draw();
	objPost2->Draw();
	//3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();

	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
}