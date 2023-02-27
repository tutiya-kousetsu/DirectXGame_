#include "Wall.h"
#include <fstream>
#include <cassert>
Wall::Wall() :Wall(Model::CreateFromOBJ("wall"))
{
	//Initializeより前に初期化とかをやるとエラーがでてくる
	//object->SetScale({ 1.0f, 1.0f, 1.0f });
}

Wall::~Wall()
{
}

void Wall::Initialize(DirectX::XMFLOAT3 position)
{
	this->position = position;
	object->SetScale({ 4.0f, 4.0f, 4.0f });
	object->SetPosition(position);
}

void Wall::Update()
{
	/*LoadWallPopData();
	UpdataWallPopCommand();*/
	
	object->Update();
}


void Wall::Draw()
{
	object->Draw();
}

void Wall::LoadWallPopData()
{
	////ファイルを開く
	//std::ifstream file;
	//file.open("Resources/ObstaclePop.csv");
	//assert(file.is_open());

	////ファイル内容を文字列ストリームにコピー
	//wallPopCom << file.rdbuf();

	////ファイルを閉じる
	//file.close();

}

void Wall::UpdataWallPopCommand()
{
	////1行分の文字列を入れる変数
	//std::string line;
	////コマンド実行ループ
	//while (getline(wallPopCom, line)) {
	//	//1行分の文字列をストリームに変換して解析しやすくする
	//	std::istringstream line_stream(line);

	//	std::string word;
	//	//,区切りで行の先頭文字列を取得
	//	getline(line_stream, word, ',');

	//	//"//"から始まる行はコメント
	//	if (word.find("//") == 0) {
	//		//コメント行は飛ばす
	//		continue;
	//	}

	//	//POPコマンド
	//	if (word.find("POP") == 0) {
	//		//x座標
	//		getline(line_stream, word, ',');
	//		float x = (float)std::atof(word.c_str());

	//		//y座標
	//		getline(line_stream, word, ',');
	//		float y = (float)std::atof(word.c_str());

	//		//z座標
	//		getline(line_stream, word, ',');
	//		float z = (float)std::atof(word.c_str());

	//		//敵を発生させる
	//		//コンストラクタ呼ぶよ
	//		Initialize(DirectX::XMFLOAT3(x, y, z));
	//		//障害物を登録する
	//	}
	//}
}
