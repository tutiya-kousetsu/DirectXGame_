#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"

#include "Enemy/EnemyBullet.h"

#include <fstream>
#include <cassert>
void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	//�X�v���C�g�̐���
	Sprite::LoadTexture(2, L"Resources/sosa_sinan.png");
	sprite = Sprite::Create(2, { 0,0 });
	sprite->SetPosition({ 0,0 });
	Sprite::LoadTexture(3, L"Resources/Life.png");
	LifeSprite = Sprite::Create(3, { 0,0 });
	Sprite::LoadTexture(4, L"Resources/Life.png");
	LifeSprite2 = Sprite::Create(4, { 26,0 });
	Sprite::LoadTexture(5, L"Resources/Life.png");
	LifeSprite3 = Sprite::Create(5, { 52,0 });

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
	camera.reset(new FollowingCamera());
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(camera.get());


	//�f�o�C�X���Z�b�g
	Fbx_Object3d::SetDevice(dxCommon->GetDev());
	//�J�������Z�b�g
	Fbx_Object3d::SetCamera(camera.get());
	//�O���t�B�b�N�X�p�C�v���C������
	Fbx_Object3d::CreateGraphicsPipeline();

	//�����̏�����
	srand((unsigned)time(NULL));

	player.reset(new Player());
	floor = new Floor();
	playerBullet = new PlayerBullet();
	enemyBullet = new EnemyBullet();
	obstacle = new Obstacle();
	for (auto i = 0; i < 14; i++) {
		enemy[i] = new Enemy();
		enemy[i]->Initialize();
	}

	//�f�[�^�ǂݍ���
	skyModel = Model::LoadFromObj("skydome");
	skyObj = Object3d::Create();
	skyObj->SetModel(skyModel);

}

void GamePlayScene::Finalize()
{
	//�X�v���C�g�ʉ��
	delete sprite;
	delete LifeSprite;
	delete LifeSprite2;
	delete LifeSprite3;

	//3D���f�����
	//3D�I�u�W�F�N�g���
	delete postEffect[0];
	delete postEffect[1];

	//3D�I�u�W�F�N�g���
	delete skyModel;
	delete skyObj;
	delete playerBullet;
	delete enemyBullet;
	for (auto i = 0; i < 14; i++) {
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
	// �}�E�X��\�����邩�ǂ���(TRUE�ŕ\���AFALSE�Ŕ�\��)
	ShowCursor(FALSE);
	// ���W�̕ύX�𔽉f
	SetCursorPos(960, 540);
	player->Updata();
	camera->SetFollowingTarget(player.get());
	//�X�V
	camera->Updata();
	
	floor->Updata();

	if (!eneFlag[0]) {
		enemy[0]->Updata();
	}
	if (eneFlag[0]) {
		enemy[1]->Updata();
		enemy[2]->Updata();
	}
	if (eneFlag[1] && eneFlag[2]) {
		enemy[3]->Updata();
		enemy[4]->Updata();
		enemy[5]->Updata();
	}
	if (eneFlag[3] && eneFlag[4] && eneFlag[5]) {
		enemy[6]->Updata();
		enemy[7]->Updata();
		enemy[8]->Updata();
		enemy[9]->Updata();
	}
	if (eneFlag[6] && eneFlag[7] && eneFlag[8] && eneFlag[9]) {
		enemy[10]->Updata();
		enemy[11]->Updata();
		enemy[12]->Updata();
		enemy[13]->Updata();
		enemy[14]->Updata();
	}


	skyObj->Updata();
	obstacle->Updata();

	CheckAllCollision();
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
	//playerBullet->Draw();
	for (auto i = 0; i < 14; i++) {
		enemy[i]->Draw();
	}

	skyObj->Draw();
	floor->Draw();
	Object3d::PostDraw();

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>
	sprite->Draw();
	if (playerLife >= 3){ LifeSprite3->Draw(); }
	if (playerLife >= 2) { LifeSprite2->Draw(); }
	if (playerLife >= 1) { LifeSprite->Draw(); }
	//spriteBG->Draw();
	// �f�o�b�O�e�L�X�g�̕`��
	//debugText->DrawAll(cmdList);

	// �X�v���C�g�`��㏈��

	Sprite::PostDraw();

	//�`��㏈��
	dxCommon->PostDraw();

}


void GamePlayScene::CheckAllCollision()
{
	//����̑Ώ�
	XMFLOAT3 posA, posB;
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();
	for (auto i = 0; i < 14; i++) {

		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy[i]->GetBullet();
#pragma region ���e�ƓG�̓����蔻��
		//if (enemy[i]->GetFrameFlag()) {
		posA = enemy[i]->GetPosition();

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			bulFlag = bullet->GetAlive();
			if (bulFlag) {
				posB = bullet->GetPosition();

				float dx = abs(posB.x - posA.x);
				float dy = abs(posB.y - posA.y);
				float dz = abs(posB.z - posA.z);

				if (dx < 1 && dy < 1 && dz < 1) {
					enemy[i]->OnCollision();
					bullet->OnCollision();
					gameScore++;
					eneFlag[i] = true;
				}
			}
		}
#pragma endregion

#pragma region �G�Ǝ��@�̓����蔻��
		posA = enemy[i]->GetPosition();

		posB = player->GetPosition();

		float dx = abs(posB.x - posA.x);
		float dy = abs(posB.y - posA.y);
		float dz = abs(posB.z - posA.z);

		if (dx < 1 && dy < 1 && dz < 1) {
			//enemy[i]->Hit();
			enemy[i]->OnCollision();
			player->OnCollision();
		}
#pragma endregion

#pragma region �G�e�Ǝ��@�̓����蔻��
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

			bulFlag = bullet->GetAlive();
			if (bulFlag) {
				posA = player->GetPosition();

				posB = bullet->GetPosition();

				float dx = abs(posB.x - posA.x);
				float dy = abs(posB.y - posA.y);
				float dz = abs(posB.z - posA.z);

				if (dx < 1 && dy < 1 && dz < 1) {
					bullet->OnCollision();
					player->OnCollision();
					playerLife--;
				}
			}
		}
#pragma endregion

	}

#pragma region yuka�Ǝ��@�̓����蔻��
	//posA = floor->GetPosition();

	//posB = player->GetPosition();

	//float dx = abs(posB.x - posA.x);
	//float dy = abs(posB.y - posA.y);
	//float dz = abs(posB.z - posA.z);

	//if (dx < 1 && dy < 1 && dz < 1) {
	//	//player->FloorCollision();
	//}
#pragma endregion

	playerPos = player->GetPosition();
	player->SetPosition(playerPos);
	//�v���C���[��HP��0�ɂȂ������ʐ؂�ւ�
	if (playerLife <= 0 || playerPos.y <= -5) {
		//�V�[���؂�ւ�
		BaseScene* scene = new GameOver();
		this->sceneManager->SetNextScene(scene);
	}
	if (eneFlag[6] && eneFlag[7] && eneFlag[8] && eneFlag[9]) {
		//�V�[���؂�ւ�
		BaseScene* scene = new GameClear();
		this->sceneManager->SetNextScene(scene);
	}
}