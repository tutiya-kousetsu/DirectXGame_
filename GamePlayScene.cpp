#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"

GamePlayScene::GamePlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{
}

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{
	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/gamePlay.png");
	//SpriteCommon::GetInstance()->LoadTexture(1, L"Resources/gamePlay.png");
	//�X�v���C�g�̐���
	sprite = Sprite::Create(1, { 0.0f,0.0f });

	Sprite::LoadTexture(2, L"Resources/tex1.png");

	Sprite::LoadTexture(100, L"Resources/white1x1.png");
	postEffect = new PostEffect();
	postEffect->Initialize();


	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

	//OBJ���烂�f���f�[�^��ǂݍ���
	modelPost = Model::LoadFromObj("post");

	//3D�I�u�W�F�N�g����
	objPost = Object3d::Create();
	objPost->SetModel(modelPost);

	//�����ǂݍ���
	//Audio::GetInstance()->SoundLoadWave("Alarm01.wav");

	//�����Đ�
	//audio->SoundPlayWave("Alarm01.wav", true);

	//�J�����̏�����
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(camera);

	//camera->SetEye({ 0, 3.0f, -7.0f });
	camera->SetTarget({ 0,2.5f,0 });
	camera->SetDistance(8.0f);
	//���f�������w�肵�ăt�@�C����ǂݍ���
	fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//�f�o�C�X���Z�b�g
	Fbx_Object3d::SetDevice(dxCommon->GetDev());
	//�J�������Z�b�g
	Fbx_Object3d::SetCamera(camera);
	//�O���t�B�b�N�X�p�C�v���C������
	Fbx_Object3d::CreateGraphicsPipeline();

	//3D�I�u�W�F�N�g�����ƃ��f���̃Z�b�g
	fbxobject1 = new Fbx_Object3d;
	fbxobject1->Initialize();
	fbxobject1->SetModel(fbxmodel1);
}

void GamePlayScene::Finalize()
{
	delete postEffect;
	delete fbxobject1;
	delete fbxmodel1;
	delete camera;
	//�X�v���C�g�ʉ��
	delete sprite1;
	delete sprite;
	//3D���f�����
	delete modelPost;
	//3D�I�u�W�F�N�g���
	delete objPost;
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();
	//objPost->SetPosition({ 0, 15,10 });
	//objPost->SetRotation({ -45, 0, 0 });
	fbxobject1->SetRotation({ 0, 90, 0 });
	if (input->PushKey(DIK_SPACE))     // �X�y�[�X�L�[��������Ă�����
	{
		fbxobject1->PlayAnimation();
	}

	//X���W,Y���W���w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);
	
	//�X�V
	objPost->Update();
	//objChr->Update();
	camera->Update();
	fbxobject1->Update();
	/*sprite->Update();
	sprite1->Update();*/
}

void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCmdList());
	// �w�i�X�v���C�g�`��
	sprite->Draw();
	sprite1->Draw();
	//�X�v���C�g�`��
	postEffect->Draw(dxCommon->GetCmdList());

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma region 3D�`��
	//3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw();

	//3D�I�u�W�F�N�g�̕`��
	//objPost->Draw();

	fbxobject1->Draw(dxCommon->GetCmdList());
	//3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
}