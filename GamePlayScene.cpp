#include "GamePlayScene.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
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

	
	//objChr->SetPosition({ +10,0,+5 });
	//�J�����̏�����
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(camera);

	//camera->SetEye({ 0, 0, -50 });
	camera->SetTarget({ 0,20,0 });
	camera->SetDistance(100.0f);
	//���f�������w�肵�ăt�@�C����ǂݍ���
	//FbxLoader::GetInstance()->LoadModelFromFile("cube");
	fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");

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
	delete fbxobject1;
	delete fbxmodel1;
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
	objPost->SetPosition({ 0, 15,10 });
	objPost->SetRotation({ -45, 0, 0 });
	//if (input->PushKey(DIK_SPACE))     // �X�y�[�X�L�[��������Ă�����
	//{

	//}

	// ���W����
	/*if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{

	}*/

	//X���W,Y���W���w�肵�ĕ\��
	DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//�X�V
	objPost->Update();
	//objChr->Update();
	camera->Update();
	fbxobject1->Update();
	sprite->Update();
	
}

void GamePlayScene::Draw()
{
	// �R�}���h���X�g�̎擾
	//ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
	//�X�v���C�g�`��
	sprite->Draw();

	//3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw();
	
	//3D�I�u�W�F�N�g�̕`��
	//objPost->Draw();
	
	fbxobject1->Draw(cmdList);
	//3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();

	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
}