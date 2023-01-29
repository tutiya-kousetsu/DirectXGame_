//#include "Floor.h"
//
////Floor::Floor() :Floor(Model::CreateFromOBJ("FloorBox"))
////{
////	object->SetScale({ 20.f, 5.0f, 20.f });
////}
//
//Floor* Floor::Create(Model* model)
//{
//	//3Dオブジェクトのインスタンスを生成
//	Floor* instance = new Floor();
//	if (instance == nullptr) {
//		return nullptr;
//	}
//
//	//初期化
//	if (!instance->Initialize()) {
//		delete instance;
//		assert(0);
//	}
//	//モデルのセット
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
