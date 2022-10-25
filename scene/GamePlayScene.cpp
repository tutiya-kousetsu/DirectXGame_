#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	if (!Sprite::LoadTexture(1, L"Resources/gamePlay.png")) {
		assert(0);
		return;
	}
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	//�X�v���C�g�̐���
	//sprite = Sprite::Create(1, { 0,0 }, false, false);
	//sprite->SetPosition({ 0,0 });

	//�X�v���C�g�̐���
	//sprite1 = Sprite::Create(2, { 0,0 }, false, false);
	// �e�N�X�`��2�Ԃɓǂݍ���
	Sprite::LoadTexture(2, L"Resources/tex1.png");
	sprite = Sprite::Create(2, { 0.0f,0.0f });

	//sprite1->SetPosition({ 0,0 });

	//�|�X�g�G�t�F�N�g�̏�����
	for (int i = 0; i <= 1; i++) {
		postEffect[i] = new PostEffect();
	}
	//�V�F�[�_�[�̑}��
	postEffect[0]->Initialize(L"Resources/shaders/PostEffectPS.hlsl");

	postEffect[1]->Initialize(L"Resources/shaders/PixelShader.hlsl");


	//�����ǂݍ���
	//Audio::GetInstance()->SoundLoadWave("Alarm01.wav");

	//�����Đ�
	//audio->SoundPlayWave("Alarm01.wav", true);

	//�J�����̏�����
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(camera);

	camera->SetEye({ 0, 10, -30 });
	camera->SetTarget({ 0,0,30 });
	camera->SetDistance(0.0f);

	//�f�o�C�X���Z�b�g
	Fbx_Object3d::SetDevice(dxCommon->GetDev());
	//�J�������Z�b�g
	Fbx_Object3d::SetCamera(camera);
	//�O���t�B�b�N�X�p�C�v���C������
	Fbx_Object3d::CreateGraphicsPipeline();

	//�����̏�����
	srand((unsigned)time(NULL));

	player = new Player();
	playerBullet = new PlayerBullet();
	for (int i = 0; i < 4; i++) {
		enemy[i] = new Enemy();
		enemy[i]->Initialize();
	}
		player->Initialize(input);
		playerBullet->Initialize(input, player);

	//�f�[�^�ǂݍ���
	groundModel = Model::LoadFromObj("ground");
	groundObj = Object3d::Create();
	groundObj->SetModel(groundModel);

	//�f�[�^�ǂݍ���
	skyModel = Model::LoadFromObj("skydome");
	skyObj = Object3d::Create();
	skyObj->SetModel(skyModel);

}

void GamePlayScene::Finalize()
{

	delete camera;
	//�X�v���C�g�ʉ��
	delete sprite1;
	delete sprite;
	//3D���f�����
	//3D�I�u�W�F�N�g���
	delete postEffect[0];
	delete postEffect[1];

	//3D�I�u�W�F�N�g���
	delete skyModel;
	delete skyObj;
	delete groundModel;
	delete groundObj;
	delete player;
	delete playerBullet;
	for (int i = 0; i < 4; i++) {
		delete enemy[i];
	}
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();

	//X���W,Y���W���w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//�X�V
	camera->Update();
	camera->Update();
	player->Update();
	playerBullet->Update();
	for (int i = 0; i < 4; i++) {
		enemy[i]->Update();
	}
	groundObj->Update();
	skyObj->Update();
	Collision();
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	//�`��O����
	dxCommon->PreDraw();
	//�X�v���C�g�`��
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCmdList());
	//�w�i�X�v���C�g�`��
	spriteBG->Draw();
	//sprite1->Draw();

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
	player->Draw();
	playerBullet->Draw();
	for (int i = 0; i < 4; i++) {
		enemy[i]->Draw();
	}
	skyObj->Draw();
	groundObj->Draw();
	Object3d::PostDraw();

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>
	//sprite->Draw();
	//spriteBG->Draw();
	// �f�o�b�O�e�L�X�g�̕`��
	//debugText->DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	
	Sprite::PostDraw();

	//�`��㏈��
	dxCommon->PostDraw();

}

void GamePlayScene::Collision()
{
	for (int i = 0; i < 4; i++) {
		//�v���C���[�ƓG�̏Փ˔���
		//�G�����݂����
		if (enemy[i]->GetFlag()) {
			//���W
			XMFLOAT3 playerPosition = player->GetPosition();
			XMFLOAT3 enemyPosition = enemy[i]->GetPosition();

			//�������߂�
			float dx = abs(playerPosition.x - enemyPosition.x);
			float dy = abs(playerPosition.y - enemyPosition.y);
			float dz = abs(playerPosition.z - enemyPosition.z);

			if (enemyPosition.z <= -5) {
				playerLife--;
				enemy[i]->Hit();
			}
		}
		//�e�ƓG�̓����蔻��
		//�G�����݂����
		if (enemy[i]->GetFrameFlag()) {
			if (enemy[i]->GetFlag()) {
				//���W
				XMFLOAT3 shootPosition = playerBullet->GetPosition();
				XMFLOAT3 enemyPosition = enemy[i]->GetPosition();

				//�������߂�
				float dx = abs(enemyPosition.x - shootPosition.x);
				float dy = abs(enemyPosition.y - shootPosition.y);
				float dz = abs(enemyPosition.z - shootPosition.z);

				if (dx < 1 && dy < 1 && dz < 1) {
					gameScore++;
					enemy[i]->frameFlag = 0;
					enemy[i]->Hit();
					playerBullet->Hit();
				}
			}
		}
	}

	//�v���C���[��HP��0�ɂȂ������ʐ؂�ւ�
	if (playerLife == 0) {
		//�V�[���؂�ւ�
		BaseScene* scene = new GameOver();
		this->sceneManager->SetNextScene(scene);
	}
	if (gameScore == 5) {
		//�V�[���؂�ւ�
		BaseScene* scene = new GameClear();
		this->sceneManager->SetNextScene(scene);
	}
}
