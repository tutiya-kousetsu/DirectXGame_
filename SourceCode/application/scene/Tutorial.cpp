#include "Tutorial.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MeshCollider.h"
#include "ParticleManager.h"
#include "Easing.h"

void Tutorial::Initialize(DirectXCommon* dxCommon)
{
	Sprite::LoadTexture(1, L"Resources/WASD_UI.png");
	wasdUI.reset(Sprite::Create(1, { 530,380 }));

	Sprite::LoadTexture(2, L"Resources/space_UI.png");
	spaceUI.reset(Sprite::Create(2, { 510,380 }));

	Sprite::LoadTexture(4, L"Resources/shot_UI.png");
	shotUI.reset(Sprite::Create(4, { 560,380 }));

	Sprite::LoadTexture(16, L"Resources/alignment.png");
	alignment.reset(Sprite::Create(16, { 600,200 }));

	//�J�����̏�����
	camera.reset(new FollowingCamera());
	//�J�����̏�����
	debugCam.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));

	//���̃I�u�W�F�N�g����
	floor.reset(TouchableObject::Create(Model::CreateFromOBJ("FloorBox")));
	floor->SetScale({ 20.f, 5.0f, 20.f });
	floor->SetPosition({ 0,-18.5f,0 });

	//�f�[�^�ǂݍ���
	skyObj = Object3d::Create();
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));

	//�f�[�^�ǂݍ���
	sceneMoveObj = Object3d::Create();
	sceneMoveObj->SetModel(Model::CreateFromOBJ("sceneMove"));
	sceneMoveObj->SetScale({ 3,3,3 });
	sceneMoveObj->SetPosition({ 0, -2.02f, 20 });

	//�f�[�^�ǂݍ���
	arrowObj = Object3d::Create();
	arrowObj->SetModel(Model::CreateFromOBJ("arrow"));
	arrowObj->SetScale({ 1.5f,1.5f,1.5f });
	arrowObj->SetPosition({ 0, 2.02f, 20 });

	//���@�̃I�u�W�F�N�g�Z�b�g+������
	player.reset(Player::Create(Model::CreateFromOBJ("octopus")));
	player->SetPosition({ 0, -1.8f, 0 });
	enemy.reset(new FrontEnemy());
	enemy->Initialize({ 0, 5, 50 });

	//�p�[�e�B�N�̃C���X�^���X
	particleMan = ParticleManager::GetInstance();
	particleMan->Initialize();
	particleMan->SetCamera(camera.get());

	postEffect.reset(new PostEffect());
	postEffect->CreateGraphicsPipelineState(L"Resources/shaders/PostEffectPS.hlsl");
	//�V�F�[�_�[�̑}��
	postEffect->Initialize();
}

void Tutorial::Finalize()
{
}

void Tutorial::Update()
{

	if (!startFlag) {
		//���S���疾�邭����
		startEfRadius += 10.5f;
		if (startEfRadius >= 1000) {
			startFlag = true;
			startEfRadius -= 10.5f;
		}
		
	}
	Input* input = Input::GetInstance();

	// �}�E�X��\�����邩�ǂ���(TRUE�ŕ\���AFALSE�Ŕ�\��)
	ShowCursor(FALSE);
	// ���W�̕ύX�𔽉f
	SetCursorPos(960, 540);
	if (!rotateFlag) {
		//�J������3D�I�u�W�F�b�g�ɃZ�b�g
		nowCamera = camera.get();
		//�J������3D�I�u�W�F�b�g�ɃZ�b�g
		Object3d::SetCamera(nowCamera);

		camera->SetFollowingTarget(player.get());

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
			// FLOAT3�ɕϊ�
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
		player->TutorialUpdate();
	}
	if (rotateFlag) {
		rotateTime--;
		//�Ǐ]�J�������畁�ʂ̃J�����ɕύX
		nowCamera = debugCam.get();
		Object3d::SetCamera(nowCamera);
		nowCamera->SetEye({ player->GetPosition().x, 2.0f, 0 });
		nowCamera->SetTarget(player->GetPosition());
		//��]������
		playerRot = player->GetRotation();
		playerRot.y += 3.0f;
		player->SetRotation(playerRot);
		if (rotateTime <= 0) {
			//�^�C����0�ɂȂ����玩�@����ɂ�����
			playerPos = player->GetPosition();
			if (easFrame < 1.0f) {
				easFrame += 0.01f;
				player->ScaleChange();
			}

			playerPos.y = Ease(In, Cubic, easFrame, playerPos.y, 40.0f);
			playerScale = player->GetScale();
			//�v���C���[�̃X�P�[����0�ɂȂ�����V�[���ړ�
			if (playerScale.x <= 0.f && playerScale.y <= 0.f &&playerScale.z <= 0.f) {
				BaseScene* scene = new GamePlayScene();
				this->sceneManager->SetNextScene(scene);
			}
			player->SetPosition(playerPos);
		}
		player->StopUpdate();
	}


	//�v���C���[���X�e�[�W���痎������X�e�[�W�̐^�񒆂ɖ߂�
	if (player->GetPosition().y <= -20) {
		player->SetPosition({ 0,10,0 });
	}

	//�e�I�u�W�F�N�g�̍X�V
	postEffect->SetRadius(startEfRadius);
	skyObj->Update();
	enemy->TitleUpdate();

	floor->Update();
	if (zonePop) {
		sceneMoveObj->Update();
	}
	arrowObj->Update();
	camera->Update();
	debugCam->Update();

	//�����蔻��
	CheckAllCollision();
	particleMan->Update();
}

void Tutorial::Draw(DirectXCommon* dxCommon)
{
	//�`��O����
	//dxCommon->PreDraw();
	//�X�v���C�g�`��
#pragma region �w�i�X�v���C�g�`��
	postEffect->PreDrawScene(dxCommon->GetCmdList());

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
	skyObj->Draw();
	enemy->Draw();

	floor->Draw();
	player->TutorialDraw(rotateFlag);

	if (zonePop) {
		sceneMoveObj->Draw();
		arrowObj->Draw();
	}
	if (!rotateFlag) {
		particleMan->Draw();
	}
	Object3d::PostDraw();

	postEffect->PostDrawScene(dxCommon->GetCmdList());

	//�`��O����
	dxCommon->PreDraw();
	postEffect->Draw(dxCommon->GetCmdList());

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>
	if (player->GetPhase() == 0) {
		wasdUI->Draw();
	}
	if (player->GetPhase() == 1) {
		spaceUI->Draw();
	}
	if (player->GetPhase() == 2) {
		shotUI->Draw();
	}
	if (!rotateFlag) {
		alignment->Draw();
	}
	
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

	//�`��㏈��
	dxCommon->PostDraw();
}

void Tutorial::CheckAllCollision()
{
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullet();

#pragma region ���e�ƓG�̓����蔻��

	Sphere pBullet;
	for (auto& pb : playerBullets) {
		if (pb->GetAlive()) {
			pBullet.center = XMLoadFloat3(&pb->GetPosition());
			pBullet.radius = pb->GetScale().x;

			//�O�̓G
			if (enemy) {
				if (enemy->GetAlive()) {
					Sphere fEnemyShape;
					fEnemyShape.center = XMLoadFloat3(&enemy->GetPosition());
					fEnemyShape.radius = enemy->GetScale().z;

					if (Collision::CheckSphere2Sphere(pBullet, fEnemyShape)) {
						pb->OnCollision();
						enemy->OnCollision();
						if (!enemy->GetAlive()) {
							zonePop = true;
						}
					}
				}
			}
		}
#pragma endregion
	}

#pragma region ���@�ƃ��[�v�]�[���̓����蔻��
	Sphere pShape;
	if (player && zonePop) {
		if (player->GetAlive()) {
			pShape.center = XMLoadFloat3(&player->GetPosition());
			pShape.radius = player->GetScale().x;
			
			Sphere zoneShape;
			if (sceneMoveObj) {
				zoneShape.center = XMLoadFloat3(&sceneMoveObj->GetPosition());
				zoneShape.radius = sceneMoveObj->GetScale().x - 2.0f;
				zoneShape.radius = sceneMoveObj->GetScale().y - 2.5f;
				if (Collision::CheckSphere2Sphere(pShape, zoneShape)) {
					rotateFlag = true;
				}
			}
		}
	}
#pragma endregion

}