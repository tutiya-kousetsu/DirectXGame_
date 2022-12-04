#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "Player.h"
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

	player = new Player();
	floor = new Floor();
	playerBullet = new PlayerBullet();
	enemyBullet = new EnemyBullet();
	obstacle = new Obstacle();

	for (auto i = 0; i < 14; i++) {
		enemy[i] = new Enemy();
		enemy[i]->Initialize();

		//�G�Ɏ��@�̃A�h���X��n���ēG�����@���g����悤�ɂ���
		enemy[i]->SetPlayer(player);
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
	delete player;
	delete skyObj;
	delete playerBullet;
	delete obstacle;
	delete enemyBullet;
	for (auto i = 0; i < 14; i++) {
		delete enemy[i];
	}
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();

	// �}�E�X��\�����邩�ǂ���(TRUE�ŕ\���AFALSE�Ŕ�\��)
	ShowCursor(FALSE);
	// ���W�̕ύX�𔽉f
	SetCursorPos(960, 540);

	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();
	float dy = mouseMove.lX * scaleY;
	angleY = -dy * XM_PI;

	{
		// �ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationY(-angleY);
		// �ݐς̉�]�s�������
		// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
		// �N�H�[�^�j�I�����g�p��������]�܂���
		matRot = matRotNew * matRot;

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// �x�N�g������]
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// ����
		float length = 0.0f;

		XMFLOAT3 target1 = camera->GetTarget();
		camera->SetEye({ target1.x + vTargetEye.m128_f32[0], target1.y + vTargetEye.m128_f32[1], target1.z + vTargetEye.m128_f32[2] });
		camera->SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });

		// �����_���炸�炵���ʒu�Ɏ��_���W������
		XMFLOAT3 target2 = camera->GetTarget();
		XMFLOAT3 eye = camera->GetEye();

		XMFLOAT3 fTargetEye = { 0.0f, 0.0f, 0.0f };
		XMVECTOR vecF = XMLoadFloat3(&fTargetEye);
		// FLOAT3�ɕϊ�
		XMStoreFloat3(&fTargetEye, vecF);
		XMVECTOR vecTarget = XMLoadFloat3(&target2);
		// FLOAT3�ɕϊ�
		XMStoreFloat3(&target2, vecTarget);
		XMVECTOR vecEye = XMLoadFloat3(&eye);
		// FLOAT3�ɕϊ�
		XMStoreFloat3(&eye, vecEye);
		// �傫���v�Z
		length = sqrtf(pow(target2.x - eye.x, 2) + pow(target2.y - eye.y, 2) + pow(target2.z - eye.z, 2));
		fTargetEye.x = eye.x - target2.x;
		fTargetEye.y = eye.y - target2.y;
		fTargetEye.z = eye.z - target2.z;

		XMFLOAT3 playerRot = player->GetRotation();
		playerRot.y = atan2f(-fTargetEye.x, -fTargetEye.z);
		playerRot.y *= 180 / XM_PI;
		player->SetRotation({ 0.0f, playerRot.y, 0.0f });
	}
	//X���W,Y���W���w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	player->Update();
	camera->SetFollowingTarget(player);
	//�X�V
	camera->Update();

	floor->Update();

	if (!eneFlag[0]) {
		enemy[0]->Update();
	}
	if (eneFlag[0]) {
		enemy[1]->Update();
		enemy[2]->Update();
	}
	if (eneFlag[1] && eneFlag[2]) {
		enemy[3]->Update();
		enemy[4]->Update();
		enemy[5]->Update();
	}
	if (eneFlag[3] && eneFlag[4] && eneFlag[5]) {
		enemy[6]->Update();
		enemy[7]->Update();
		enemy[8]->Update();
		enemy[9]->Update();
	}
	if (eneFlag[6] && eneFlag[7] && eneFlag[8] && eneFlag[9]) {
		enemy[10]->Update();
		enemy[11]->Update();
		enemy[12]->Update();
		enemy[13]->Update();
		enemy[14]->Update();
	}

	skyObj->Update();
	obstacle->Update();

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
	obstacle->Draw();
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
	if (playerLife >= 3) { LifeSprite3->Draw(); }
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


//void GamePlayScene::CheckAllCollision()
//{
//	//����̑Ώ�
//	XMFLOAT3 posA, posB;
//	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();
//	for (auto i = 0; i < 14; i++) {
//
//		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy[i]->GetBullet();
//#pragma region ���e�ƓG�̓����蔻��
//		//if (enemy[i]->GetFrameFlag()) {
//		posA = enemy[i]->GetPosition();
//
//		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
//			bulFlag = bullet->GetAlive();
//			if (bulFlag) {
//				posB = bullet->GetPosition();
//
//				float dx = abs(posB.x - posA.x);
//				float dy = abs(posB.y - posA.y);
//				float dz = abs(posB.z - posA.z);
//
//				if (dx < 1 && dy < 1 && dz < 1) {
//					enemy[i]->OnCollision();
//					bullet->OnCollision();
//					gameScore++;
//					eneFlag[i] = true;
//				}
//			}
//		}
//#pragma endregion
//
//#pragma region �G�Ǝ��@�̓����蔻��
//		posA = enemy[i]->GetPosition();
//
//		posB = player->GetPosition();
//
//		float dx = abs(posB.x - posA.x);
//		float dy = abs(posB.y - posA.y);
//		float dz = abs(posB.z - posA.z);
//
//		if (dx < 1 && dy < 1 && dz < 1) {
//			//enemy[i]->Hit();
//			enemy[i]->OnCollision();
//			player->OnCollision();
//		}
//#pragma endregion
//
//#pragma region �G�e�Ǝ��@�̓����蔻��
//		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
//
//			bulFlag = bullet->GetAlive();
//			if (bulFlag) {
//				posA = player->GetPosition();
//
//				posB = bullet->GetPosition();
//
//				float dx = abs(posB.x - posA.x);
//				float dy = abs(posB.y - posA.y);
//				float dz = abs(posB.z - posA.z);
//
//				if (dx < 1 && dy < 1 && dz < 1) {
//					bullet->OnCollision();
//					player->OnCollision();
//					playerLife--;
//				}
//			}
//		}
//#pragma endregion
//
//	}
//
//#pragma region yuka�Ǝ��@�̓����蔻��
//	//posA = floor->GetPosition();
//
//	//posB = player->GetPosition();
//
//	//float dx = abs(posB.x - posA.x);
//	//float dy = abs(posB.y - posA.y);
//	//float dz = abs(posB.z - posA.z);
//
//	//if (dx < 1 && dy < 1 && dz < 1) {
//	//	//player->FloorCollision();
//	//}
//#pragma endregion
//
//	playerPos = player->GetPosition();
//	player->SetPosition(playerPos);
//	//�v���C���[��HP��0�ɂȂ������ʐ؂�ւ�
//	if (playerLife <= 0 || playerPos.y <= -5) {
//		//�V�[���؂�ւ�
//		BaseScene* scene = new GameOver();
//		this->sceneManager->SetNextScene(scene);
//	}
//	if (eneFlag[6] && eneFlag[7] && eneFlag[8] && eneFlag[9]) {
//		//�V�[���؂�ւ�
//		BaseScene* scene = new GameClear();
//		this->sceneManager->SetNextScene(scene);
//	}
//}