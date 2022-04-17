#include "DebugText.h"

DebugText* DebugText::GetInstance()
{
	static DebugText instance;

	return &instance;
}

void DebugText::Initialize(SpriteCommon* spriteCommon, UINT texnumber)
{
	//nullptr�`�F�b�N
	assert(spriteCommon);

	//�����������o�ϐ��Ɋi�[
	this->spriteCommon = spriteCommon;

	//�S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(this->sprites); i++)
	{
		//�X�v���C�g���Đ�
		this->sprites[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

void DebugText::Print(const std::string& text, float x, float y, float scale)
{
	//�S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		//�ő啶��������
		if (this->spriteIndex >= maxCharCount) {
			break;
		}

		//1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		//ASCII�R�[�h��2�i����΂����ԍ����v�Z
		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		//���W�v�Z
		this->sprites[this->spriteIndex]->SetPosition({ x + fontWidth * scale * i, y, 0 });
		this->sprites[this->spriteIndex]->SetTexLeftTop({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight });
		this->sprites[this->spriteIndex]->SetTexSize({ fontWidth, fontHeight });
		this->sprites[this->spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });
		//���_�o�b�t�@�]��
		this->sprites[this->spriteIndex]->TransferVertexBuffer();
		//�X�V
		this->sprites[this->spriteIndex]->Update();

		//������1�i�߂�
		this->spriteIndex++;
	}
}

void DebugText::DrawAll()
{
	//�S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < this->spriteIndex; i++)
	{
		//�X�v���C�g�`��
		this->sprites[i]->Draw();
	}

	this->spriteIndex = 0;
}