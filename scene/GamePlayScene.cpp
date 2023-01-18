#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "Player.h"
#include "Enemy/EnemyBullet.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "Line.h"
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
	for (auto i = 0; i < 4; i++) {
		obstacle[i] = new Obstacle();
	}
	particleMan = ParticleManager::GetInstance();
	//line = new Line();
	particleMan->Initialize();
	particleMan->SetCamera(camera.get());
	enemy = new Enemy();
	//enemy->Initialize();
	for (int i = 0; i < 3; i++) {
		//�O
		frontEnemy[i] = new FrontEnemy();
		frontEnemy[i]->Initialize();
		//��
		leftEnemy[i] = new LeftEnemy();
		leftEnemy[i]->Initialize();
		//�E
		rightEnemy[i] = new RightEnemy();
		rightEnemy[i]->Initialize();
		//���
		backEnemy[i] = new BackEnemy();
		backEnemy[i]->Initialize();
		//�G�Ɏ��@�̃A�h���X��n���ēG�����@���g����悤�ɂ���
		//enemy->SetPlayer(player);
		frontEnemy[i]->SetPlayer(player);
		leftEnemy[i]->SetPlayer(player);
		rightEnemy[i]->SetPlayer(player);
		backEnemy[i]->SetPlayer(player);
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

	//3D�I�u�W�F�N�g���
	delete skyModel;
	delete player;
	delete skyObj;
	delete playerBullet;
	for (auto i = 0; i < 4; i++) {
		delete obstacle[i];
	}
	delete enemyBullet;
	delete enemy;
	for (int i = 0; i < 3; i++) {
		delete frontEnemy[i];
		delete leftEnemy[i];
		delete rightEnemy[i];
		delete backEnemy[i];
	}
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();

	// �}�E�X��\�����邩�ǂ���(TRUE�ŕ\���AFALSE�Ŕ�\��)
	ShowCursor(FALSE);
	// ���W�̕ύX�𔽉f
	SetCursorPos(960, 540);


	camera->SetFollowingTarget(player);

	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();
	float dy = mouseMove.lX * scaleY;
	angleY = -dy * XM_PI;

	{
		// �ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationY(-angleY);
		// �ݐς̉�]�s�������
		matRot = matRotNew * matRot;

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 0.5f, 0.0f, 0.0f };

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
		// FLOAT3�ɕϊ�//
		XMStoreFloat3(&fTargetEye, vecF);
		XMVECTOR vecTarget = XMLoadFloat3(&target2);
		// FLOAT3�ɕϊ�
		XMStoreFloat3(&target2, vecTarget);
		XMVECTOR vecEye = XMLoadFloat3(&eye);
		// FLOAT3�ɕϊ�
		XMStoreFloat3(&eye, vecEye);
		//���K��
		fTargetEye.x = eye.x - target2.x;
		fTargetEye.y = eye.y - target2.y;
		fTargetEye.z = eye.z - target2.z;

		//�v���C���[�̉�]
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

	particleMan->Update();
	//�X�V
	camera->Update();

	floor->Update();
	//line->Update();
	/*if (!fEneFlag[0]) {
		enemy[0]->Update();
	}
	if (fEneFlag[0]) {
		enemy[1]->Update();
		enemy[2]->Update();

	}
	if (fEneFlag[1] && fEneFlag[2]) {
		enemy[3]->Update();
		enemy[4]->Update();
		enemy[5]->Update();
	}
	if (fEneFlag[3] && fEneFlag[4] && fEneFlag[5]) {
		enemy[6]->Update();
		enemy[7]->Update();
		enemy[8]->Update();
		enemy[9]->Update();
	}*/

	//for (int i = 0; i < 3; i++) {
	if (enemyScene == 0) {
		if (fEneFlag >= 0) {
			//���������
			frontEnemy[0]->resurrection(3);
			frontEnemy[0]->Update();
		}
		if (fEneFlag >= 1) {
			frontEnemy[1]->resurrection(2);
			frontEnemy[1]->Update();
		}
		if (fEneFlag >= 2) {
			frontEnemy[2]->Update();
		}
	}
	else if (enemyScene == 1) {
		leftEnemy[0]->Update();
	}
	else if (enemyScene == 2) {
		rightEnemy[0]->Update();
	}
	else if (enemyScene == 3) {
		backEnemy[0]->Update();
	}

	//}
	skyObj->Update();
	for (auto i = 0; i < 4; i++) {
		obstaclePos[i] = obstacle[i]->GetPosition();
		obstacle[0]->SetPosition({ 13,0,13 });
		obstacle[1]->SetPosition({ -13,0,-13 });
		obstacle[2]->SetPosition({ -13,0,13 });
		obstacle[3]->SetPosition({ 13,0,-13 });

		obstacle[i]->Update();
	}
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
	/*for (auto i = 0; i < 14; i++) {
		enemy[i]->Draw();
	}*/
	//enemy->Draw();
	for (int i = 0; i < 3; i++) {

		frontEnemy[i]->Draw();
		leftEnemy[i]->Draw();
		rightEnemy[i]->Draw();
		backEnemy[i]->Draw();
	}
	for (auto i = 0; i < 4; i++) {
		obstacle[i]->Draw();
	}
	//line->Draw();
	skyObj->Draw();
	floor->Draw();
	particleMan->Draw();
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

//�O�G�̒e�̓����蔻��
void GamePlayScene::FrontColl()
{
	for (int i = 0; i < 3; i++) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = frontEnemy[i]->GetBullet();
#pragma region �G�e�Ǝ��@�̓����蔻��
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

			Sphere eBullet;

			for (auto& eb : enemyBullets) {
				if (eb->GetAlive()) {
					eBullet.center = XMLoadFloat3(&eb->GetPosition());
					eBullet.radius = eb->GetScale().x;
					Sphere playerShape;
					playerShape.center = XMLoadFloat3(&player->GetPosition());
					playerShape.radius = player->GetScale().x;

					if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
						player->OnCollision();
						eb->OnCollision();

					}
				}
			}
		}

#pragma endregion

#pragma region �G�e�Ə�Q���̓����蔻��
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

			Sphere eBullet;

			for (auto& eb : enemyBullets) {
				if (eb->GetAlive()) {
					eBullet.center = XMLoadFloat3(&eb->GetPosition());
					eBullet.radius = eb->GetScale().x;
					if (frontEnemy[i]->GetAlive()) {
						Sphere obstacleShape;
						for (auto j = 0; j < 4; j++) {
							obstacleShape.center = XMLoadFloat3(&obstacle[j]->GetPosition());
							obstacleShape.radius = obstacle[j]->GetScale().x;

							if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
								eb->OnCollision();
								//obstacle->OnCollision();
							}
						}
					}

				}
			}
		}

#pragma endregion

	}
}

//���G�̒e�̓����蔻��
void GamePlayScene::LeftColl()
{
	for (int i = 0; i < 3; i++) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = leftEnemy[i]->GetBullet();
#pragma region �G�e�Ǝ��@�̓����蔻��
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

			Sphere eBullet;

			for (auto& eb : enemyBullets) {
				if (eb->GetAlive()) {
					eBullet.center = XMLoadFloat3(&eb->GetPosition());
					eBullet.radius = eb->GetScale().x;
					Sphere playerShape;
					playerShape.center = XMLoadFloat3(&player->GetPosition());
					playerShape.radius = player->GetScale().x;

					if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
						eb->OnCollision();
						//particleMan->CreateParticle(pos, 70, 4, 1.65f);
						player->OnCollision();
						playerLife--;
					}
				}
			}
		}

#pragma endregion

#pragma region �G�e�Ə�Q���̓����蔻��
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

			Sphere eBullet;

			for (auto& eb : enemyBullets) {
				if (eb->GetAlive()) {
					eBullet.center = XMLoadFloat3(&eb->GetPosition());
					eBullet.radius = eb->GetScale().x;
					if (frontEnemy[i]->GetAlive()) {
						Sphere obstacleShape;
						for (auto j = 0; j < 4; j++) {
							obstacleShape.center = XMLoadFloat3(&obstacle[j]->GetPosition());
							obstacleShape.radius = obstacle[j]->GetScale().x;

							if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
								eb->OnCollision();
								//obstacle->OnCollision();
							}
						}
					}

				}
			}
		}

#pragma endregion

	}
}

//�E�G�̒e�̓����蔻��
void GamePlayScene::RightColl()
{
	for (int i = 0; i < 3; i++) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = rightEnemy[i]->GetBullet();
#pragma region �G�e�Ǝ��@�̓����蔻��
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

			Sphere eBullet;

			for (auto& eb : enemyBullets) {
				if (eb->GetAlive()) {
					eBullet.center = XMLoadFloat3(&eb->GetPosition());
					eBullet.radius = eb->GetScale().x;
					Sphere playerShape;
					playerShape.center = XMLoadFloat3(&player->GetPosition());
					playerShape.radius = player->GetScale().x;

					if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
						eb->OnCollision();
						//particleMan->CreateParticle(pos, 70, 4, 1.65f);
						player->OnCollision();
						playerLife--;
					}
				}
			}
		}

#pragma endregion

#pragma region �G�e�Ə�Q���̓����蔻��
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

			Sphere eBullet;

			for (auto& eb : enemyBullets) {
				if (eb->GetAlive()) {
					eBullet.center = XMLoadFloat3(&eb->GetPosition());
					eBullet.radius = eb->GetScale().x;
					if (frontEnemy[i]->GetAlive()) {
						Sphere obstacleShape;
						for (auto j = 0; j < 4; j++) {
							obstacleShape.center = XMLoadFloat3(&obstacle[j]->GetPosition());
							obstacleShape.radius = obstacle[j]->GetScale().x;

							if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
								eb->OnCollision();
								//obstacle->OnCollision();
							}
						}
					}

				}
			}
		}

#pragma endregion

	}
}

//���G�̒e�̓����蔻��
void GamePlayScene::BackColl()
{
	for (int i = 0; i < 3; i++) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = backEnemy[i]->GetBullet();
#pragma region �G�e�Ǝ��@�̓����蔻��
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

			Sphere eBullet;

			for (auto& eb : enemyBullets) {
				if (eb->GetAlive()) {
					eBullet.center = XMLoadFloat3(&eb->GetPosition());
					eBullet.radius = eb->GetScale().x;
					Sphere playerShape;
					playerShape.center = XMLoadFloat3(&player->GetPosition());
					playerShape.radius = player->GetScale().x;

					if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
						eb->OnCollision();
						//particleMan->CreateParticle(pos, 70, 4, 1.65f);
						player->OnCollision();
						playerLife--;
					}
				}
			}
		}

#pragma endregion

#pragma region �G�e�Ə�Q���̓����蔻��
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

			Sphere eBullet;

			for (auto& eb : enemyBullets) {
				if (eb->GetAlive()) {
					eBullet.center = XMLoadFloat3(&eb->GetPosition());
					eBullet.radius = eb->GetScale().x;
					if (frontEnemy[i]->GetAlive()) {
						Sphere obstacleShape;
						for (auto j = 0; j < 4; j++) {
							obstacleShape.center = XMLoadFloat3(&obstacle[j]->GetPosition());
							obstacleShape.radius = obstacle[j]->GetScale().x;

							if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
								eb->OnCollision();
								//obstacle->OnCollision();
							}
						}
					}

				}
			}
		}

#pragma endregion

	}
}

void GamePlayScene::CheckAllCollision()
{
	FrontColl();
	LeftColl();
	RightColl();
	BackColl();

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();
	for (int i = 0; i < 3; i++) {

#pragma region ���e�ƓG�̓����蔻��

		Sphere pBullet;

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			for (auto& pb : playerBullets) {
				if (pb->GetAlive()) {
					pBullet.center = XMLoadFloat3(&pb->GetPosition());
					pBullet.radius = pb->GetScale().x;

					//�O�̓G
					if (frontEnemy[i]->GetAlive()) {
						Sphere fEnemyShape;
						fEnemyShape.center = XMLoadFloat3(&frontEnemy[i]->GetPosition());
						fEnemyShape.radius = frontEnemy[i]->GetScale().z;

						if (Collision::CheckSphere2Sphere(pBullet, fEnemyShape)) {
							pb->OnCollision();
							frontEnemy[i]->OnCollision();
							if (!frontEnemy[i]->GetAlive()) {
								fEneFlag++;
								fEneTimer++;
							}
						}
					}

					//���̓G
					if (leftEnemy[i]->GetAlive()) {
						Sphere lEnemyShape;
						lEnemyShape.center = XMLoadFloat3(&leftEnemy[i]->GetPosition());
						lEnemyShape.radius = leftEnemy[i]->GetScale().z;

						if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
							pb->OnCollision();
							leftEnemy[i]->OnCollision();
						}
					}

					//�E�̓G
					if (rightEnemy[i]->GetAlive()) {
						Sphere lEnemyShape;
						lEnemyShape.center = XMLoadFloat3(&rightEnemy[i]->GetPosition());
						lEnemyShape.radius = rightEnemy[i]->GetScale().z;

						if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
							pb->OnCollision();
							rightEnemy[i]->OnCollision();
						}
					}
					//���̓G
					if (backEnemy[i]->GetAlive()) {
						Sphere lEnemyShape;
						lEnemyShape.center = XMLoadFloat3(&backEnemy[i]->GetPosition());
						lEnemyShape.radius = backEnemy[i]->GetScale().z;

						if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
							pb->OnCollision();
							backEnemy[i]->OnCollision();
						}
					}
				}
			}
		}


#pragma endregion

#pragma region ���e�Ə�Q���̓����蔻��
		for (auto& pb : playerBullets) {
			if (pb->GetAlive()) {
				pBullet.center = XMLoadFloat3(&pb->GetPosition());
				pBullet.radius = pb->GetScale().x;
				Sphere obstacleShape;
				for (auto i = 0; i < 4; i++) {

					obstacleShape.center = XMLoadFloat3(&obstacle[i]->GetPosition());
					obstacleShape.radius = obstacle[i]->GetScale().x;


					if (Collision::CheckSphere2Sphere(pBullet, obstacleShape)) {
						pb->OnCollision();
					}
				}
			}
		}
	}
#pragma endregion

#pragma region ���Ǝ��@�̓����蔻��
	Plane floorShape;
	floorShape.normal = XMVectorSet(0, 1, 0, 0);
	floorShape.distance = -1.0f;

	Sphere playerShape;
	playerShape.center = XMLoadFloat3(&player->GetPosition());
	playerShape.radius = player->GetScale().z;

	if (Collision::CheckSphere2Plane(playerShape, floorShape)) {
		player->FloorCollision();
	}
#pragma endregion
	XMFLOAT3 playerPos = player->GetPosition();

	//�v���C���[��HP��0�ɂȂ������ʐ؂�ւ�
	if (playerLife <= 0 || playerPos.y <= -5) {
		//�V�[���؂�ւ�
		BaseScene* scene = new GameOver();
		this->sceneManager->SetNextScene(scene);
	}
	//if (fEneFlag[6] && fEneFlag[7] && fEneFlag[8] && fEneFlag[9]) {
	//	//�V�[���؂�ւ�
	//	BaseScene* scene = new GameClear();
	//	this->sceneManager->SetNextScene(scene);
	//}
}