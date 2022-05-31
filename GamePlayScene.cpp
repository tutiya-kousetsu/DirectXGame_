#include "GamePlayScene.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"

void GamePlayScene::Initialize()
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
	objPost2 = Object3d::Create();
	objPost2->SetModel(modelPost);
	//�J�����̏�����
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(camera);

	camera->SetEye({ 0, 0, -50 });
	camera->SetTarget({ 0,0,-10 });

	//���f�������w�肵�ăt�@�C����ǂݍ���
	FbxLoader::GetInstance()->LoadModelFromFile("cube");

	//�v���C���[�̏����ʒu
	playerPos = { 0, 0, 0 };
	//playerPos2 = { -15, 0, 0 };

	// �S�Ă̋��̏����l
	playerPos2 = { 0, 15, 0 };
	m = 1.f;
	v = { 0, 0, 0 };
	a = { 0, -g, 0 };
	r = 1.0f;


}

void GamePlayScene::Finalize()
{
	//3D�I�u�W�F�N�g���
	delete objPost;
	delete objPost2;
	//3D���f�����
	delete modelPost;
	//�J�����̉��
	delete camera;
	//�X�v���C�g���
	delete sprite;
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();
	objPost->SetScale({ 1.5f, 1.5f, 1.5f });
	objPost2->SetScale({ 3.0f, 3.0f, 3.0f });

	/*���@�̓���*/
	r = Angle * 3.14f / 180.0f;	//�x���@�̊p�x���ʓx�@�ɕϊ�

	float addX = cos(r) * Length;		//�O�p�֐����g�p���A�~�̈ʒu������o���B
	float addY = sin(r) * Length;

	playerPos.x = centerX + addX;						//���ʂŏo�Ă����ʒu�𒆐S�ʒu�ɉ��Z���A�����`��ʒu�Ƃ���
	playerPos.y = centerY + addY;
	Angle += 3.0f;							//�p�x�X�V�A���v���
	//// �l���X�V(����)
	//v.y += a.y;
	//playerPos2.y += v.y;

	//v.z += a.z;
	//playerPos2.z += v.z;

	//v.x += a.x;
	//playerPos2.x += v.x;

	//// -10�ȉ��Ɉړ������璵�˕Ԃ�
	//if (playerPos2.y < -10) {
	//	playerPos2.y -= v.y;
	//	v.y *= -repulCoe;
	//}
	objPost->SetPosition(playerPos);
	objPost2->SetPosition(playerPos2);

	//X���W,Y���W���w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Hello,DirectX!!", 0, 0);
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Nihon Kogakuin", 0, 20, 2.0f);

	//�X�V
	objPost->Update();
	//objPost2->Update();
	//objChr->Update();
	camera->Update();
	sprite->Update();
}

void GamePlayScene::Draw()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
	//�X�v���C�g�`��
	//sprite->Draw();

	//3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw();

	//3D�I�u�W�F�N�g�̕`��
	objPost->Draw();
	objPost2->Draw();
	//3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();

	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
}