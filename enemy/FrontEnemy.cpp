#include "FrontEnemy.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "Player.h"
#include "ParticleManager.h"

FrontEnemy::~FrontEnemy()
{
}

bool FrontEnemy::Initialize()
{
	object.reset(new EnemyObject());
	object->Initialize(Model::CreateFromOBJ("BlueBox"));

	SetScale({ 1.0f, 1.0f, 1.0f });
	object->SetRotation({ 0, 180, 0 });
	particleMan = ParticleManager::GetInstance();
	// ���݂̍��W���擾
	position = GetPosition();
	int x = rand() % 700;
	float x2 = (float)x / 10 - 35;//10�`-10�͈̔�
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0�͈̔�
	int z = rand() % 700;
	//float z2 = (float)z / 10 - 35;//6~0�͈̔�
	position = { x2, 35, 35 };
	// ���W�̕ύX�𔽉f
	SetPosition(position);
	AccessPhase();

	return true;
}

void FrontEnemy::Update()
{
	if (alive) {
		appearance();

		if (!appFlag) {
			/*SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
			assert(sphereCollider);
			Ray ray;
			ray.start = sphereCollider->center;
			ray.start.m128_f32[1] += sphereCollider->GetRadius();
			ray.dir = { 0,-1,0,0 };
			RaycastHit raycastHit;
			if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_ALLIES, &raycastHit, sphereCollider->GetRadius())) {
				OnCollision();
			}*/
			//�G���~�܂�����t���O�𗧂ĂĒe�������n�߂�
			Shoot();
		}
		object->SetPosition(position);
	}

	object->Update();
}

void FrontEnemy::appearance()
{
	//�`�悳�ꂽ��A�G�������_���Ō��߂��ʒu�̍����܂ł��낷
	position.y -= moveY;
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0�͈̔�
	if (position.y <= y2) {
		moveY = 0;
		appFlag = false;
	}
}

void FrontEnemy::FrontShoot()
{
	//player�Ɍ������Ēe����
	{
		assert(this->player);

		XMVECTOR playerPos = player->GetWorldPosition();
		XMVECTOR enemyPos = GetWorldPosition();

		//���x���v�Z
		//��������W�I�܂ł̃x�N�g��
		velocity = {
			playerPos.m128_f32[0] - enemyPos.m128_f32[0],
			playerPos.m128_f32[1] - enemyPos.m128_f32[1],
			playerPos.m128_f32[2] - enemyPos.m128_f32[2]
		};
		//�傫����1�ɂ���(�x�N�g���𐳋K�����ĕԂ��Ă�����֐�)
		velocity = XMVector3Normalize(velocity);
		//�傫����C�ӂ̒l�ɂ���
		velocity = XMVectorScale(velocity, 1.5f);

		//�W�I�Ɍ�����
		float rotx = atan2f(velocity.m128_f32[1], velocity.m128_f32[2]);
		float roty = atan2f(velocity.m128_f32[0], velocity.m128_f32[2]);
	}
	//�R���X�g���N�^�ĂԂ�
	EnemyBullet* newBullet = new EnemyBullet();
	//������
	newBullet->Initialize(position, velocity);

	bullets.reset(newBullet);

}

void FrontEnemy::Shoot()
{
	shootTimer--;
	if (shootTimer < 0) {
		FrontShoot();
		shootTimer = kShootInterval;
	}
	if (bullets) {
		bullets->Update();
	}
}

void FrontEnemy::OnCollision()
{
	for (int j = 0; j < 100; j++) {
		DirectX::XMFLOAT3 pos = object->GetPosition();
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float md_vel = 0.20f;
		DirectX::XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		DirectX::XMFLOAT3 acc{};
		const float rnd_acc = 0.005f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
		//�ǉ�
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}

	life--;
	if (life == 0) {
		alive = false;

	}
}

void FrontEnemy::Draw()
{
	//�t���O1�œG�\��
	if (alive) {
		object->Draw();
		if (bullets) {
			bullets->Draw();
		}
	}
}

XMVECTOR FrontEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	//worldPos��player��position�������
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}

void FrontEnemy::AccessPhase()
{
	shootTimer = kShootInterval;
}