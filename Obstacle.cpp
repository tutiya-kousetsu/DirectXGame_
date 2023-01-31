#include "Obstacle.h"
#include "SphereCollider.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

Obstacle::Obstacle()
{
}

void Obstacle::Initialize(DirectX::XMFLOAT3 pos)
{
	object.reset(new Object3d());
	object->Initialize();
	object->SetModel(Model::CreateFromOBJ("obstacle"));
	object->SetScale({ 3.f, 3.f, 3.f });
	object->SetCollider(new SphereCollider);
	object->SetPosition(pos);
}

void Obstacle::Update()
{
	
	object->Update();
}

void Obstacle::Draw()
{
	object->Draw();
}

void Obstacle::OnCollision()
{
}

void Obstacle::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->collider = collider;
	//�R���W�����}�l�[�W���ɓo�^
	CollisionManager::GetInstance()->AddCollider(collider);
	UpdateWorldMatrix();
	//�R���C�_�[���X�V���Ă���
	collider->Update();
}
