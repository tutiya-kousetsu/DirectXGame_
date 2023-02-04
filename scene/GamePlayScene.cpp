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
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MeshCollider.h"
#include "TouchableObject.h"
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

	//player = new Player();
	//floor = new Floor();
	playerBullet = new PlayerBullet();
	enemyBullet = new EnemyBullet();
	collisionMan = CollisionManager::GetInstance();
	player = player->Create(Model::CreateFromOBJ("PlayerRed"));
	//���̃I�u�W�F�N�g����
	floorModel = Model::CreateFromOBJ("FloorBox");
	floor = TouchableObject::Create(floorModel);
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });

	//obModel = Model::CreateFromOBJ("obstacle");
	
	particleMan = ParticleManager::GetInstance();
	//line = new Line();
	particleMan->Initialize();
	particleMan->SetCamera(camera.get());
	enemy = new Enemy();
	for (int i = 0; i < 11; i++) {
		//�O
		frontEnemy[i] = new FrontEnemy();
		frontEnemy[i]->Initialize();
		//�G�Ɏ��@�̃A�h���X��n���ēG�����@���g����悤�ɂ���
		frontEnemy[i]->SetPlayer(player);
	}
	for (int i = 0; i < 7; i++) {
		//��
		leftEnemy[i] = new LeftEnemy();
		leftEnemy[i]->Initialize();
		//�G�Ɏ��@�̃A�h���X��n���ēG�����@���g����悤�ɂ���
		leftEnemy[i]->SetPlayer(player);
	}
	for (int i = 0; i < 4; i++) {
		//�E
		rightEnemy[i] = new RightEnemy();
		rightEnemy[i]->Initialize();
		//�G�Ɏ��@�̃A�h���X��n���ēG�����@���g����悤�ɂ���
		rightEnemy[i]->SetPlayer(player);
	}
	for (int i = 0; i < 2; i++) {
		//���
		backEnemy[i] = new BackEnemy();
		backEnemy[i]->Initialize();
		//�G�Ɏ��@�̃A�h���X��n���ēG�����@���g����悤�ɂ���
		backEnemy[i]->SetPlayer(player);
	}
	//�f�[�^�ǂݍ���
	skyModel = Model::CreateFromOBJ("skydome");
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

	//3D�I�u�W�F�N�g���
	delete skyModel;
	delete floor;
	delete player;
	delete skyObj;
	delete playerBullet;
	delete enemyBullet;
	delete enemy;
	for (int i = 0; i < 11; i++) {
		delete frontEnemy[i];
	}
	for (int i = 0; i < 7; i++) {
		delete leftEnemy[i];
	}
	for (int i = 0; i < 4; i++) {
		delete rightEnemy[i];
	}
	for (int i = 0; i < 2; i++) {
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

	//fbxObj->Update();

	//�X�V
	camera->Update();
	floor->Update();

	//�G�̔������鏇��
	if (fEneFlag >= 0) {
		frontEnemy[0]->Update();
	}
	if (fEneFlag >= 1) {
		frontEnemy[1]->Update();
		frontEnemy[2]->Update();
	}
	if (fEneFlag >= 3 && lEneFlag >= 0) {
		playerLife = 3;
		frontEnemy[3]->Update();
		frontEnemy[4]->Update();
		leftEnemy[0]->Update();
	}
	if (fEneFlag >= 5 && lEneFlag >= 1) {
		frontEnemy[5]->Update();
		frontEnemy[6]->Update();
		leftEnemy[1]->Update();
		leftEnemy[2]->Update();
	}
	if (fEneFlag >= 7 && lEneFlag >= 3 && rEneFlag >= 0) {
		playerLife = 3;
		frontEnemy[7]->Update();
		frontEnemy[8]->Update();
		leftEnemy[3]->Update();
		leftEnemy[4]->Update();
		rightEnemy[0]->Update();
		rightEnemy[1]->Update();
	}
	if (fEneFlag >= 7 && lEneFlag >= 5 && rEneFlag >= 2) {
		playerLife = 3;
		frontEnemy[9]->Update();
		frontEnemy[10]->Update();
		leftEnemy[5]->Update();
		leftEnemy[6]->Update();
		rightEnemy[2]->Update();
		rightEnemy[3]->Update();
		backEnemy[0]->Update();
		backEnemy[1]->Update();
	}

	skyObj->Update();
	LoadObstaclePopData();
	UpdataObstaclePopCommand();

	collisionMan->CheckAllCollisions();
	//CheckAllCollision();

	particleMan->Update();
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
	for (int i = 0; i < 11; i++) {
		frontEnemy[i]->Draw();
	}
	for (int i = 0; i < 7; i++) {
		leftEnemy[i]->Draw();
	}
	for (int i = 0; i < 4; i++) {
		rightEnemy[i]->Draw();
	}
	for (int i = 0; i < 2; i++) {
		backEnemy[i]->Draw();
	}
	for (auto& obstacle : obstacles) {
		obstacle->Draw();
	}
	//fbxObj->Draw(dxCommon->GetCmdList());
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

void GamePlayScene::LoadObstaclePopData()
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/ObstaclePop.csv");
	assert(file.is_open());

	//�t�@�C�����e�𕶎���X�g���[���ɃR�s�[
	obstaclePopCom << file.rdbuf();

	//�t�@�C�������
	file.close();
}

void GamePlayScene::UpdataObstaclePopCommand()
{
	//1�s���̕����������ϐ�
	std::string line;
	//�R�}���h���s���[�v
	while (getline(obstaclePopCom, line)) {
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		std::string word;
		//,��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�s�̓R�����g
		if (word.find("//") == 0) {
			//�R�����g�s�͔�΂�
			continue;
		}

		//POP�R�}���h
		if (word.find("POP") == 0) {
			//x���W
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y���W
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z���W
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//�G�𔭐�������
			//�R���X�g���N�^�ĂԂ�
			std::unique_ptr<Obstacle> newObstacle = std::make_unique<Obstacle>();
			newObstacle->Initialize(XMFLOAT3(x, y, z));
			//��Q����o�^����
			obstacles.push_back(std::move(newObstacle));
		}
	}
	for (auto& obstacle : obstacles) {
		obstacle->Update();
	}
}

//�O�G�̒e�̓����蔻��
//void GamePlayScene::FrontColl()
//{
//	for (int i = 0; i < 11; i++) {
//		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = frontEnemy[i]->GetBullet();
//#pragma region �G�e�Ǝ��@�̓����蔻��
//		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
//
//			Sphere eBullet;
//
//			for (auto& eb : enemyBullets) {
//				if (eb->GetAlive()) {
//					eBullet.center = XMLoadFloat3(&eb->GetPosition());
//					eBullet.radius = eb->GetScale().x;
//					Sphere playerShape;
//					playerShape.center = XMLoadFloat3(&player->GetPosition());
//					playerShape.radius = player->GetScale().x;
//
//					if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
//						player->CreateParticle();
//						eb->OnCollision();
//						playerLife--;
//					}
//				}
//			}
//
//#pragma endregion
//
//#pragma region �G�e�Ə�Q���̓����蔻��
//			for (auto& eb : enemyBullets) {
//				if (eb->GetAlive()) {
//					eBullet.center = XMLoadFloat3(&eb->GetPosition());
//					eBullet.radius = eb->GetScale().x;
//					if (eb->GetAlive()) {
//
//						//for (auto& obstacle : obstacles) {
//						//	Sphere obstacleShape;
//						//	obstacleShape.center = XMLoadFloat3(&obstacle->);
//						//	obstacleShape.radius = obstacle->GetScale().x;
//
//						//	if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
//						//		eb->OnCollision();
//						//		//obstacle->OnCollision();
//						//	}
//						//}
//					}
//				}
//			}
//#pragma endregion
//		}
//	}
//}
//
////���G�̒e�̓����蔻��
//void GamePlayScene::LeftColl()
//{
//	for (int i = 0; i < 7; i++) {
//		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = leftEnemy[i]->GetBullet();
//#pragma region �G�e�Ǝ��@�̓����蔻��
//		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
//
//			Sphere eBullet;
//
//			for (auto& eb : enemyBullets) {
//				if (eb->GetAlive()) {
//					eBullet.center = XMLoadFloat3(&eb->GetPosition());
//					eBullet.radius = eb->GetScale().x;
//					Sphere playerShape;
//					playerShape.center = XMLoadFloat3(&player->GetPosition());
//					playerShape.radius = player->GetScale().x;
//
//					if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
//						eb->OnCollision();
//						player->CreateParticle();
//						playerLife--;
//					}
//				}
//			}
//
//#pragma endregion
//
//#pragma region �G�e�Ə�Q���̓����蔻��
//			for (const std::unique_ptr<Obstacle>& obstacle : obstacles) {
//				Sphere eBullet;
//				for (auto& eb : enemyBullets) {
//					if (eb->GetAlive()) {
//						eBullet.center = XMLoadFloat3(&eb->GetPosition());
//						eBullet.radius = eb->GetScale().x;
//						if (leftEnemy[i]->GetAlive()) {
//							Sphere obstacleShape;
//							//for (auto& ob : obstacles) {
//							//	obstacleShape.center = XMLoadFloat3(&ob->GetPosition());
//							//	obstacleShape.radius = ob->GetScale().x;
//
//							//	if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
//							//		eb->OnCollision();
//							//		//obstacle->OnCollision();
//							//	}
//							//}
//						}
//					}
//				}
//			}
//		}
//
//#pragma endregion
//
//	}
//}
//
////�E�G�̒e�̓����蔻��
//void GamePlayScene::RightColl()
//{
//	for (int i = 0; i < 4; i++) {
//		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = rightEnemy[i]->GetBullet();
//#pragma region �G�e�Ǝ��@�̓����蔻��
//		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
//
//			Sphere eBullet;
//
//			for (auto& eb : enemyBullets) {
//				if (eb->GetAlive()) {
//					eBullet.center = XMLoadFloat3(&eb->GetPosition());
//					eBullet.radius = eb->GetScale().x;
//					Sphere playerShape;
//					playerShape.center = XMLoadFloat3(&player->GetPosition());
//					playerShape.radius = player->GetScale().x;
//
//					if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
//						eb->OnCollision();
//						player->CreateParticle();
//						playerLife--;
//					}
//				}
//			}
//		}
//
//#pragma endregion
//
//#pragma region �G�e�Ə�Q���̓����蔻��
//		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
//
//			Sphere eBullet;
//
//			for (auto& eb : enemyBullets) {
//				if (eb->GetAlive()) {
//					eBullet.center = XMLoadFloat3(&eb->GetPosition());
//					eBullet.radius = eb->GetScale().x;
//					if (rightEnemy[i]->GetAlive()) {
//						Sphere obstacleShape;
//						//for (auto& ob : obstacles) {
//						//	obstacleShape.center = XMLoadFloat3(&ob->GetPosition());
//						//	obstacleShape.radius = ob->GetScale().x;
//
//						//	if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
//						//		eb->OnCollision();
//						//		//obstacle->OnCollision();
//						//	}
//						//}
//					}
//
//				}
//			}
//		}
//
//#pragma endregion
//
//	}
//}
//
////���G�̒e�̓����蔻��
//void GamePlayScene::BackColl()
//{
//	for (int i = 0; i < 2; i++) {
//		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = backEnemy[i]->GetBullet();
//#pragma region �G�e�Ǝ��@�̓����蔻��
//		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
//
//			Sphere eBullet;
//
//			for (auto& eb : enemyBullets) {
//				if (eb->GetAlive()) {
//					eBullet.center = XMLoadFloat3(&eb->GetPosition());
//					eBullet.radius = eb->GetScale().x;
//					Sphere playerShape;
//					playerShape.center = XMLoadFloat3(&player->GetPosition());
//					playerShape.radius = player->GetScale().x;
//
//					if (Collision::CheckSphere2Sphere(eBullet, playerShape)) {
//						eb->OnCollision();
//						player->CreateParticle();
//						playerLife--;
//					}
//				}
//			}
//		}
//
//#pragma endregion
//
//#pragma region �G�e�Ə�Q���̓����蔻��
//		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
//
//			Sphere eBullet;
//
//			for (auto& eb : enemyBullets) {
//				if (eb->GetAlive()) {
//					eBullet.center = XMLoadFloat3(&eb->GetPosition());
//					eBullet.radius = eb->GetScale().x;
//					if (backEnemy[i]->GetAlive()) {
//						Sphere obstacleShape;
//						//for (auto& ob : obstacles) {
//						//	obstacleShape.center = XMLoadFloat3(&ob->GetPosition());
//						//	obstacleShape.radius = ob->GetScale().x;
//
//						//	if (Collision::CheckSphere2Sphere(eBullet, obstacleShape)) {
//						//		eb->OnCollision();
//						//		//obstacle->OnCollision();
//						//	}
//						//}
//					}
//
//				}
//			}
//		}
//
//#pragma endregion
//
//	}
//}
//
void GamePlayScene::CheckAllCollision()
{
//	FrontColl();
//	LeftColl();
//	RightColl();
//	BackColl();
//
//	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();
//
//#pragma region ���e�ƓG�̓����蔻��
//
//	Sphere pBullet;
//
//	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
//		for (auto& pb : playerBullets) {
//			if (pb->GetAlive()) {
//				pBullet.center = XMLoadFloat3(&pb->GetPosition());
//				pBullet.radius = pb->GetScale().x;
//
//				//�O�̓G
//				for (int i = 0; i < 11; i++) {
//
//					if (frontEnemy[i]->GetAlive()) {
//						Sphere fEnemyShape;
//						fEnemyShape.center = XMLoadFloat3(&frontEnemy[i]->GetPosition());
//						fEnemyShape.radius = frontEnemy[i]->GetScale().z;
//
//						if (Collision::CheckSphere2Sphere(pBullet, fEnemyShape)) {
//							pb->OnCollision();
//							frontEnemy[i]->OnCollision();
//							if (!frontEnemy[i]->GetAlive()) {
//								fEneFlag++;
//							}
//						}
//					}
//				}
//				for (int i = 0; i < 7; i++) {
//					//���̓G
//					if (leftEnemy[i]->GetAlive()) {
//						Sphere lEnemyShape;
//						lEnemyShape.center = XMLoadFloat3(&leftEnemy[i]->GetPosition());
//						lEnemyShape.radius = leftEnemy[i]->GetScale().z;
//
//						if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
//							pb->OnCollision();
//							leftEnemy[i]->OnCollision();
//							if (!leftEnemy[i]->GetAlive()) {
//								lEneFlag++;
//							}
//						}
//					}
//				}
//				for (int i = 0; i < 4; i++) {
//
//					//�E�̓G
//					if (rightEnemy[i]->GetAlive()) {
//						Sphere lEnemyShape;
//						lEnemyShape.center = XMLoadFloat3(&rightEnemy[i]->GetPosition());
//						lEnemyShape.radius = rightEnemy[i]->GetScale().z;
//
//						if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
//							pb->OnCollision();
//							rightEnemy[i]->OnCollision();
//							if (!rightEnemy[i]->GetAlive()) {
//								rEneFlag++;
//							}
//						}
//					}
//				}
//				for (int i = 0; i < 2; i++) {
//
//					//���̓G
//					if (backEnemy[i]->GetAlive()) {
//						Sphere lEnemyShape;
//						lEnemyShape.center = XMLoadFloat3(&backEnemy[i]->GetPosition());
//						lEnemyShape.radius = backEnemy[i]->GetScale().z;
//
//						if (Collision::CheckSphere2Sphere(pBullet, lEnemyShape)) {
//							pb->OnCollision();
//							backEnemy[i]->OnCollision();
//							if (!backEnemy[i]->GetAlive()) {
//								bEneFlag++;
//							}
//						}
//					}
//				}
//			}
//		}
//
//
//#pragma endregion
//
//#pragma region ���e�Ə�Q���̓����蔻��
//		for (const std::unique_ptr<Obstacle>& obstacle : obstacles) {
//			for (auto& pb : playerBullets) {
//				if (pb->GetAlive()) {
//					pBullet.center = XMLoadFloat3(&pb->GetPosition());
//					pBullet.radius = pb->GetScale().x;
//					Sphere obstacleShape;
//					//for (auto& ob : obstacles) {
//					//	obstacleShape.center = XMLoadFloat3(&ob->GetPosition());
//					//	obstacleShape.radius = ob->GetScale().x;
//
//					//	if (Collision::CheckSphere2Sphere(pBullet, obstacleShape)) {
//					//		pb->OnCollision();
//					//		//obstacle->OnCollision();
//					//	}
//					//}
//				}
//			}
//		}
//	}
#pragma endregion

	XMFLOAT3 playerPos = player->GetPosition();

	//�v���C���[��HP��0�ɂȂ������ʐ؂�ւ�
	if (playerLife <= 0 || playerPos.y <= -5) {
		//�V�[���؂�ւ�
		BaseScene* scene = new GameOver();
		this->sceneManager->SetNextScene(scene);
	}
	if (fEneFlag >= 11 && lEneFlag >= 7 && rEneFlag >= 4 && bEneFlag >= 2) {
		//�V�[���؂�ւ�
		BaseScene* scene = new GameClear();
		this->sceneManager->SetNextScene(scene);
	}
}