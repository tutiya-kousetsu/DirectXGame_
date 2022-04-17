#include "MyGame.h"

void MyGame::Initialize()
{
	//���N���X�̏�����
	Framework::Initialize();

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
	//�X�v���C�g�ʉ��
	for (auto& sprite : sprites)
	{
		delete sprite;
	}
	//delete sprite;
	//3D���f�����
	delete modelPost;
	//3D�I�u�W�F�N�g���
	delete objPost;
	//���N���X�̏I������
	Framework::Finalize();
}

void MyGame::Update()
{
	//���N���X�̍X�V����
	Framework::Update();

	if (input->PushKey(DIK_SPACE))     // �X�y�[�X�L�[��������Ă�����
	{

	}

	// ���W����
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{

	}

	//X���W,Y���W���w�肵�ĕ\��
	debugText->Print("Hello,DirectX!!", 0, 0);
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	debugText->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//�X�V
	objPost->Update();
	//objChr->Update();
	camera->Update();
	//sprite->Update();
}

void MyGame::Draw()
{
	//�`��O����
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
