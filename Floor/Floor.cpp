//#include "Floor.h"
//
////Floor::Floor() :Floor(Model::CreateFromOBJ("FloorBox"))
////{
////	object->SetScale({ 20.f, 5.0f, 20.f });
////}
//
//Floor* Floor::Create(Model* model)
//{
//	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
//	Floor* instance = new Floor();
//	if (instance == nullptr) {
//		return nullptr;
//	}
//
//	//������
//	if (!instance->Initialize()) {
//		delete instance;
//		assert(0);
//	}
//	//���f���̃Z�b�g
//	if (model) {
//		instance->SetModel(model);
//	}
//
//	return instance;
//}
//
//bool Floor::Initialize(Model* model)
//{
//	if (!TouchableObject::Initialize(model)) {
//		return false;
//	}
//}
//
//
//void Floor::Update()
//{
//	position = Object3d::GetPosition();
//	position = {0,-18.5f,0};
//	Object3d::SetScale({ 20.f, 5.0f, 20.f });
//	Object3d::SetPosition(position);
//	Object3d::Update();
//}
//
//void Floor::OnCollision(const CollisionInfo& info)
//{
//}
//
//void Floor::Draw()
//{
//	Object3d::Draw();
//}
