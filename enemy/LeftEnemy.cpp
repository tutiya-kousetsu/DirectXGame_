#include "LeftEnemy.h"
#include "Player.h"

//LeftEnemy::LeftEnemy()
//{
//
//	SetScale({ 1.0f, 1.0f, 1.0f });
//
//	// ���݂̍��W���擾
//	position = GetPosition();
//	int x = rand() % 700;
//	float x2 = (float)x / 10 - 35;//10�`-10�͈̔�
//	int y = rand() % 70;
//	float y2 = (float)y / 10;//6~0�͈̔�
//	int z = rand() % 600;
//	float z2 = (float)z / 10 - 30;//30~-30�͈̔�
//	position = { -35, 35, z2 };
//	// ���W�̕ύX�𔽉f
//	SetPosition(position);
//}

//LeftEnemy* LeftEnemy::Create(Model* model)
//{
//	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
//	LeftEnemy* instance = new LeftEnemy();
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
//
//	//return nullptr;
//}


LeftEnemy::~LeftEnemy()
{
}

bool LeftEnemy::Initialize()
{
	///enemy = enemy->Create(Model::CreateFromOBJ("BlueBox"));

	AccessPhase();
	return true;
}

void LeftEnemy::Update()
{
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});
	if (alive) {
		appearance();
		if (!appFlag) {
			Shoot();

			//���ړ�
			position.z += move;
			if (position.z >= 30) {
				move = move * -1;
			}
			if (position.z <= -30) {
				move = move * -1;
			}
		}
		object->SetPosition(position);
	}

	object->Update();
}

void LeftEnemy::appearance()
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

void LeftEnemy::LeftShoot()
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
	//�R���X�g���N�^�Ă�
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//������
	newBullet->Initialize(position, velocity);
	//�e��o�^����
	bullets.push_back(std::move(newBullet));
}

void LeftEnemy::Shoot()
{
	shootTimer--;
	if (shootTimer < 0) {
		LeftShoot();
		shootTimer = kShootInterval;
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}
}

void LeftEnemy::Draw()
{
	//�t���O1�œG�\��
	if (alive) {
		object->Draw();
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw();
		}
	}
}


XMVECTOR LeftEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	//worldPos��player��position�������
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}

void LeftEnemy::AccessPhase()
{
	shootTimer = kShootInterval;
}