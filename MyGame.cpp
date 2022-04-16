#include "MyGame.h"

void MyGame::Initialize()
{
	//WindowsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

	//DirectX�̏�����
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	//�f�o�b�N�e�L�X�g
	debugText = new DebugText();
	//�f�o�b�N�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	const int debugTextTexNumber = 2;
	//�f�o�b�N�e�L�X�g�p�̃e�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	//�f�o�b�N�e�L�X�g������
	debugText->Initialize(spriteCommon, debugTextTexNumber);

	//���͂̏�����
	input = new Input();
	input->Initialize(winApp);

	//�I�[�f�B�I�̏�����
	audio = new Audio();
	audio->Initialize();

	//3D�I�u�W�F�b�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDev(), winApp->window_width, winApp->window_height);
	//�J�����̏�����
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(camera);

#pragma region �V�[������������
	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(0, L"Resources/texture.png");
	spriteCommon->LoadTexture(1, L"Resources/house.png");

	//�X�v���C�g�̐���
	Sprite* sprite = Sprite::Create(spriteCommon, 0);
	sprites.push_back(sprite);
	sprite->SetPosition({ 500,300,0 });

	//OBJ���烂�f���f�[�^��ǂݍ���
	modelPost = Model::LoadFromObj("post");

	//3D�I�u�W�F�N�g����
	objPost = Object3d::Create();
	objPost->SetModel(modelPost);

	//�����ǂݍ���
	//audio->SoundLoadWave("Alarm01.wav");

	//�����Đ�
	//audio->SoundPlayWave("Alarm01.wav", true);

	objPost->SetPosition({ -10,0,-5 });
	//objChr->SetPosition({ +10,0,+5 });

	camera->SetTarget({ -10,0,-5 });
	camera->SetEye({ 0, 0, -50 });
}

void MyGame::Finalize()
{
	winApp->Finalize();
#pragma endregion WindowsAPI��n��
	//�e����
	//�f�o�b�N�e�L�X�g���
	delete debugText;
	//�X�v���C�g�ʉ��
	for (auto& sprite : sprites)
	{
		delete sprite;
	}
	//delete sprite;
	//�X�v���C�g���ʕ����
	delete spriteCommon;
	//3D���f�����
	delete modelPost;
	//3D�I�u�W�F�N�g���
	delete objPost;
	//�I�[�f�B�I���
	delete audio;
	//DirectX���
	delete dxCommon;
	//���͊J��
	delete input;
	//WindowsAPI���
	delete winApp;

}

void MyGame::Update()
{
#pragma region �E�B���h�E���b�Z�[�W����
	if (winApp->ProcessMessage()) {
		//�Q�[�����[�v�𔲂���
		this->endRequst = true;
		return;
	}

	input->Update();

	if (input->PushKey(DIK_SPACE))     // �X�y�[�X�L�[��������Ă�����
	{

	}

	// ���W����
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{

	}

	//X���W,Y���W���w�肵�ĕ\��
	debugText->Print("Hello,DirectX!!", 200, 100);
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	debugText->Print("Nihon Kogakuin", 200, 200, 2.0f);

	//�X�V
	objPost->Update();
	//objChr->Update();
	camera->Update();
	//sprite->Update();
}

void MyGame::Draw()
{
	dxCommon->PreDraw();

	//3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(dxCommon->GetCmdList());
	//3D�I�u�W�F�N�g�̕`��
	objPost->Draw();

	//3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();

	//�X�v���C�g���ʃR�}���h
	spriteCommon->PreDraw();

	//�X�v���C�g�`��
	//sprite->Draw();

	//�f�o�b�N�e�L�X�g�`��
	debugText->DrawAll();

	//�`��㏈��
	dxCommon->PostDraw();

}
