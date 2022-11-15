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
	if (!Sprite::LoadTexture(1, L"Resources/gamePlay.png")) {
		assert(0);
		return;
	}
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	//�X�v���C�g�̐���
	Sprite::LoadTexture(2, L"Resources/sosa_sinan.png");
	sprite = Sprite::Create(2, { 0,0 });
	sprite->SetPosition({ 0,0 });

	// �e�N�X�`��2�Ԃɓǂݍ���
	/*Sprite::LoadTexture(2, L"Resources/tex1.png");
	sprite = Sprite::Create(2, { 0.0f,0.0f });*/


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
	for (auto i = 0; i < 9; i++) {
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
	//3D���f�����
	//3D�I�u�W�F�N�g���
	delete postEffect[0];
	delete postEffect[1];

	//3D�I�u�W�F�N�g���
	delete skyModel;
	delete skyObj;
	delete playerBullet;
	delete enemyBullet;
	for (auto i = 0; i < 9; i++) {
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
	camera->SetFollowingTarget(player.get());
	//�X�V
	camera->Updata();
	player->Updata();
	floor->Updata();
	//playerBullet->Update();


	for (auto i = 0; i < 9; i++) {
		flag[i] = enemy[i]->GetAlive();
		
	}
	if (flag[0] == true) {
		enemy[0]->Updata();
	}
	if (flag[0] == false) {
		flagTimer++;
	}
	if (flagTimer >= 300) {
		//flag[0] = true;
		flag[1] = true;
	}

	if (flag[1] && !flag[0] && flagTimer >= 300) {
		enemy[1]->Updata();
		enemy[2]->Updata();
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
	spriteBG->Draw();
	

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
	for (auto i = 0; i < 9; i++) {
		flag[i] = enemy[i]->GetAlive();
		if (flag[0] == true) {
			enemy[0]->Draw();
		}
		if (flag[0] == false) {
			flagTimer++;
		}
		if (flagTimer >= 300) {
			flag[1] = true;
		}

		if (flag[1] && !flag[0] && flagTimer >= 300) {
			enemy[1]->Draw();
			enemy[2]->Draw();
		}
	}
	obstacle->Draw();
	skyObj->Draw();
	//groundObj->Draw();
	floor->Draw();
	Object3d::PostDraw();

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>
	sprite->Draw();
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
	for (auto i = 0; i < 9; i++) {

		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy[i]->GetBullet();
#pragma region ���e�ƓG�̓����蔻��
		//if (enemy[i]->GetFrameFlag()) {
		posA = enemy[i]->GetPosition();

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			posB = bullet->GetPos();

			float dx = abs(posB.x - posA.x);
			float dy = abs(posB.y - posA.y);
			float dz = abs(posB.z - posA.z);

			if (dx < 1 && dy < 1 && dz < 1) {
				enemy[i]->OnCollision();

				bullet->OnCollision();
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

//void GamePlayScene::LoadEnemyPopData()
//{
//	//�t�@�C�����J��
//	std::ifstream file;
//	file.open("Resources/enemyPop.csv");
//	assert(file.is_open());
//
//	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
//	enemyPopCommands << file.rdbuf();
//
//	//�t�@�C�������
//	file.close();
//}

//void GamePlayScene::UpdataEnemyPopCommand()
//{
//	//1�s���̕����s�������ϐ�
//	std::string line;
//	//�R�}���h���s���[�v
//	while (getline(enemyPopCommands, line)) {
//		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
//		std::istringstream line_stream(line);
//
//		std::string word;
//		//,��؂�ōs�̐擪�������擾
//		getline(line_stream, word, ',');
//
//		//"//"����n�܂�s�̓R�����g
//		if (word.find("//") == 0) {
//			//�R�����g�̍s���΂�
//			continue;
//		}
//
//		//POP�R�}���h
//		if (word.find("POP") == 0) {
//			//x���W
//			getline(line_stream, word, ',');
//			float x = (float)std::atof(word.c_str());
//
//			//y���W
//			getline(line_stream, word, ',');
//			float y = (float)std::atof(word.c_str());
//			
//			//z���W
//			getline(line_stream, word, ',');
//			float z = (float)std::atof(word.c_str());
//
//			//�G�𔭐�������
//
//		}
//	}
//}