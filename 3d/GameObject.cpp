#include "GameObject.h"

using namespace DirectX;

GameObject::GameObject(Model* model, const DirectX::XMFLOAT3& position) :object(std::move(Object3d::Create()))
{
	object->SetModel(model);
	object->SetPosition(position);
}

void GameObject::Update()
{
	object->Update();
}

void GameObject::Draw()
{
	if (alive) {
		object->Draw();
	}
}