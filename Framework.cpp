#include "Framework.h"
#include "FbxLoader.h"

void Framework::Run()
{
	//�Q�[���̏�����
	Initialize();

	while (true)  // �Q�[�����[�v
	{
		//���t���[���X�V
		Update();
		//�I�����N�G�X�g�������甲����
		if (IsEndRequst()) {
			break;
		}
		//�`��
		Draw();
	}
	// �Q�[���̏I��
	Finalize();
}

void Framework::Initialize()
{
	//WindowsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

	//DirectX�̏�����
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

	//�X�v���C�g���ʕ����̏�����
	spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	//�f�o�b�N�e�L�X�g
	debugText = DebugText::GetInstance();
	//�f�o�b�N�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	const int debugTextTexNumber = 2;
	//�f�o�b�N�e�L�X�g�p�̃e�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	//�f�o�b�N�e�L�X�g������
	debugText->Initialize(spriteCommon, debugTextTexNumber);

	//���͂̏�����
	input = Input::GetInstance();
	input->Initialize(winApp);

	SpriteCommon::GetInstance()->LoadTexture(100, L"Resources/white1x1.png");
	postEffect = new PostEffect();
	postEffect->Initialize();

	//�I�[�f�B�I�̏�����
	/*audio = Audio::GetInstance();
	audio->Initialize();*/

	//3D�I�u�W�F�b�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDev(),dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);
	//�V�[���}�l�[�M���[�̐���
	this->sceneManager = new SceneManager();
}

void Framework::Finalize()
{
	delete postEffect;
	//�V�[���}�l�[�W���[���
	delete sceneManager;
	//�V���O���g���ɂ�����delete�_��
	//DirectX���
	delete dxCommon;
	//WindowsAPI�̏I������
	winApp->Finalize();
	//WindowsAPI���
	delete winApp;
	//FBX�̉��
	FbxLoader::GetInstance()->Finalize();

}

void Framework::Update()
{
#pragma region �E�B���h�E���b�Z�[�W����
	if (winApp->ProcessMessage()) {
		//�Q�[�����[�v�𔲂���
		this->endRequst = true;
		return;
	}
	//���͂̍X�V
	input->Update();
	/// <summary>
	/// �V�[���̍X�V
	/// </summary>
	this->sceneManager->Update(dxCommon);
}

void Framework::Draw()
{
	//�`��O����
	dxCommon->PreDraw();

	//�V�[���`��
	this->sceneManager->Draw(dxCommon);
	postEffect->Draw(dxCommon->GetCmdList());
	//�f�o�b�N�e�L�X�g�`��
	//debugText->DrawAll();

	//�`��㏈��
	dxCommon->PostDraw();
}
