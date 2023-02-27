#include "Wall.h"
#include <fstream>
#include <cassert>
Wall::Wall() :Wall(Model::CreateFromOBJ("wall"))
{
	//Initialize���O�ɏ������Ƃ������ƃG���[���łĂ���
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
	////�t�@�C�����J��
	//std::ifstream file;
	//file.open("Resources/ObstaclePop.csv");
	//assert(file.is_open());

	////�t�@�C�����e�𕶎���X�g���[���ɃR�s�[
	//wallPopCom << file.rdbuf();

	////�t�@�C�������
	//file.close();

}

void Wall::UpdataWallPopCommand()
{
	////1�s���̕����������ϐ�
	//std::string line;
	////�R�}���h���s���[�v
	//while (getline(wallPopCom, line)) {
	//	//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
	//	std::istringstream line_stream(line);

	//	std::string word;
	//	//,��؂�ōs�̐擪��������擾
	//	getline(line_stream, word, ',');

	//	//"//"����n�܂�s�̓R�����g
	//	if (word.find("//") == 0) {
	//		//�R�����g�s�͔�΂�
	//		continue;
	//	}

	//	//POP�R�}���h
	//	if (word.find("POP") == 0) {
	//		//x���W
	//		getline(line_stream, word, ',');
	//		float x = (float)std::atof(word.c_str());

	//		//y���W
	//		getline(line_stream, word, ',');
	//		float y = (float)std::atof(word.c_str());

	//		//z���W
	//		getline(line_stream, word, ',');
	//		float z = (float)std::atof(word.c_str());

	//		//�G�𔭐�������
	//		//�R���X�g���N�^�ĂԂ�
	//		Initialize(DirectX::XMFLOAT3(x, y, z));
	//		//��Q����o�^����
	//	}
	//}
}
