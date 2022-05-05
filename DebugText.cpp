#include "DebugText.h"

DebugText* DebugText::GetInstance()
{
	static DebugText instance;

	return &instance;
}

void DebugText::Initialize(SpriteCommon* spriteCommon, UINT texnumber)
{
	//nullptrチェック
	assert(spriteCommon);

	//引数をメンバ変数に格納
	this->spriteCommon = spriteCommon;

	//全てのスプライトデータについて
	for (int i = 0; i < _countof(this->sprites); i++)
	{
		//スプライトを再生
		this->sprites[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

void DebugText::Print(const std::string& text, float x, float y, float scale)
{
	//全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		//最大文字数超過
		if (this->spriteIndex >= maxCharCount) {
			break;
		}

		//1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		//ASCIIコードの2段分飛ばした番号を計算
		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		//座標計算
		this->sprites[this->spriteIndex]->SetPosition({ x + fontWidth * scale * i, y, 0 });
		this->sprites[this->spriteIndex]->SetTexLeftTop({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight });
		this->sprites[this->spriteIndex]->SetTexSize({ fontWidth, fontHeight });
		this->sprites[this->spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });
		//頂点バッファ転送
		this->sprites[this->spriteIndex]->TransferVertexBuffer();
		//更新
		this->sprites[this->spriteIndex]->Update();

		//文字を1つ進める
		this->spriteIndex++;
	}
}

void DebugText::DrawAll()
{
	//全ての文字のスプライトについて
	for (int i = 0; i < this->spriteIndex; i++)
	{
		//スプライト描画
		this->sprites[i]->Draw();
	}

	this->spriteIndex = 0;
}