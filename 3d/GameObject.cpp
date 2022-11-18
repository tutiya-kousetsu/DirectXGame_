#include "GameObject.h"

using namespace DirectX;

GameObject::GameObject(Model* model, const DirectX::XMVECTOR& position) :object(std::move(Object3d::Create()))
{
	object->SetModel(model);
	object->SetPosition(position);
}

void GameObject::Updata()
{
	object->Updata();
}

void GameObject::Draw()
{
	if (alive) {
		object->Draw();
	}
}