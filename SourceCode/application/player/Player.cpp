#include "Player.h"
#include "Input.h"
#include "ParticleManager.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"
Player* Player::Create(Model* model)
{
	//3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}
	//モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

Player::~Player()
{
}

bool Player::Initialize()
{
	if (!Object3d::Initialize()) {
		return false;
	}
	particleMan = ParticleManager::GetInstance();

	Object3d::SetRotation({ 0,0,0 });
	Object3d::SetScale({ 0.9f,0.9f,0.9f });
	camera.reset(new FollowingCamera());
	shake.reset(new Shake());
	//コライダーの追加
	float radius = 1.9f;
	//半径だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(DirectX::XMVECTOR({ 0, radius, 0, 0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}

void Player::Update()
{
	if (life >= 0) {
		//弾のフラグがfalseになったら削除する
		bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
			return !bullet->GetAlive();
			});
		move();
		jump();

		Input* input = Input::GetInstance();
		if (input->TriggerMouseLeft()) {
			//フェーズ切り替え中は撃てなくする
			if (!phaseFlag) {
				Shoot();
			}
		}
		for (std::unique_ptr<PlayerBullet>& bul : bullets) {
			bul->Update();
		}
		Object3d::Update();
	}
	if (life <= 0) {
		alive = false;
	}

	Object3d::SetPosition(position);
}

//動かしたくない時用のアップデート
void Player::StopUpdate()
{
	Object3d::Update();
}

//マウス関数
void Player::Mouse()
{
	//インスタンスを持ってきてinputクラスを使えるようにする
	Input* input = Input::GetInstance();

	Input::MouseMove mouseMove = input->GetMouseMove();
	float dy = mouseMove.lX * scaleY;
	angleY = -dy * XM_PI;
	{
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationY(-angleY);
		// 累積の回転行列を合成
		matRot = matRotNew * matRot;

		// 注視点から視点へのベクトルと、上方向ベクトル
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 0.5f, 0.0f, 0.0f };

		// ベクトルを回転
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		XMFLOAT3 target1 = camera->GetTarget();
		camera->SetEye({ target1.x + vTargetEye.m128_f32[0], target1.y + vTargetEye.m128_f32[1], target1.z + vTargetEye.m128_f32[2] });
		camera->SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });

		// 注視点からずらした位置に視点座標を決定
		XMFLOAT3 target2 = camera->GetTarget();
		XMFLOAT3 eye = camera->GetEye();
		XMFLOAT3 fTargetEye = { 0.0f, 0.0f, 0.0f };
		//正規化
		fTargetEye.x = eye.x - target2.x;
		fTargetEye.y = eye.y - target2.y;
		fTargetEye.z = eye.z - target2.z;

		//プレイヤーの回転
		XMFLOAT3 playerRot = Object3d::GetRotation();
		playerRot.y = atan2f(-fTargetEye.x, -fTargetEye.z);
		playerRot.y *= 180 / XM_PI;
		Object3d::SetRotation({ 0.0f, playerRot.y, 0.0f });
	}
}


//移動
void Player::move(float speed)
{
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	DirectX::XMFLOAT3 position = Object3d::GetPosition();
	float moveSpeed = 0.3f;
	// 前方向と横方向の単位ベクトルを作る
	XMVECTOR forwardVec = XMVectorSet(0, 0, 1, 1);
	XMVECTOR horizontalVec = XMVectorSet(1, 0, 0, 1);
	// プレイヤーの回転に合わせて回転させる(前後)
	forwardVec = XMVector3Rotate(forwardVec, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(Object3d::GetRotation().x),
		XMConvertToRadians(Object3d::GetRotation().y),
		XMConvertToRadians(Object3d::GetRotation().z)));
	// プレイヤーの回転に合わせて回転させる(横)
	horizontalVec = XMVector3Rotate(horizontalVec, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(Object3d::GetRotation().x),
		XMConvertToRadians(Object3d::GetRotation().y),
		XMConvertToRadians(Object3d::GetRotation().z)));
	// 大きさをmoveSpeedにする
	forwardVec = XMVectorScale(forwardVec, moveSpeed);
	horizontalVec = XMVectorScale(horizontalVec, moveSpeed);

	if (input->PushKey(DIK_W)) {
		position.x += forwardVec.m128_f32[0];
		position.x += forwardVec.m128_f32[1];
		position.z += forwardVec.m128_f32[2];
		moveFlag = true;
	}
	else if (input->PushKey(DIK_S)) {
		position.x -= forwardVec.m128_f32[0];
		position.x -= forwardVec.m128_f32[1];
		position.z -= forwardVec.m128_f32[2];
		moveFlag = true;
	}

	if (input->PushKey(DIK_D)) {
		position.x += horizontalVec.m128_f32[0];
		position.x += horizontalVec.m128_f32[1];
		position.z += horizontalVec.m128_f32[2];
		moveFlag = true;
	}
	else if (input->PushKey(DIK_A)) {
		position.x -= horizontalVec.m128_f32[0];
		position.x -= horizontalVec.m128_f32[1];
		position.z -= horizontalVec.m128_f32[2];
		moveFlag = true;
	}
	if (moveFlag) {
		operatePhase = 1;
	}
	// 座標の変更を反映
	Object3d::SetPosition(position);

}

//ジャンプ
void Player::jump()
{
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	position = Object3d::GetPosition();

	//落下処理
	if (!onGround) {
		//下向きの加速度
		const float fallAcc = -0.021f;
		const float fallVYMin = -0.5f;
		//加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);

		//移動
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}
	//ジャンプ操作
	else if (input->TriggerMouseRight() || input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		jumpOpFlag = true;
		const float jumpVYFist = 0.56f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}
	if (jumpOpFlag) {
		operatePhase = 2;
	}

	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };
			//排斥方向
			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値角度
			const float threshold = cosf(XMConvertToRadians(0.0f));
			//角度差によって天井、地面と判定される場合を除いて
			if (-threshold < cos && cos < threshold) {
				//球を押し出す
				sphere->center += info.reject;
				move += info.reject;
			}
			return true;
		}

		Sphere* sphere = nullptr;
		//押し出しによる移動量
		DirectX::XMVECTOR move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	//コライダー更新
	UpdateWorldMatrix();
	collider->Update();

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_ENEMYS);

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;
	if (onGround) {
		const float adsDistance = 0.18f;
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.4f + adsDistance)) {
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.4f);
			// 行列の更新など
			Object3d::Update();
		}
		// 地面がないので落下
		else {
			onGround = false;
			fallV = {};
		}
	}
	// 落下状態
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.4f)) {
			// 着地
			onGround = true;
		}
	}
	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	// 行列の更新など
	Object3d::Update();
}

//スケールを小さくする用関数
void Player::ScaleSmall()
{
	scale = Object3d::GetScale();
	if (scale.x >= 0.f && scale.y >= 0.f && scale.z >= 0.f) {
		scale.x -= 0.01f;
		scale.y -= 0.01f;
		scale.z -= 0.01f;
		Object3d::SetScale(scale);
	}
}

//スケールを大きくする用関数
void Player::ScaleLarge()
{
	scale = Object3d::GetScale();
	if (scale.x <= 0.9f && scale.y <= 0.9f && scale.z <= 0.9f) {
		scale.x += 0.01f;
		scale.y += 0.01f;
		scale.z += 0.01f;
		Object3d::SetScale(scale);
	}

}

//弾発射
bool Player::Shoot()
{
	const float kBulletSpeed = 1.0f;
	XMVECTOR velocity = XMVectorSet(0, 0, kBulletSpeed, 1);

	velocity = XMVector3TransformNormal(velocity, Object3d::GetMatWorld());

	//コンストラクタ呼ぶよ
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//初期化
	newBullet->Initialize({ position.x - 0.45f, position.y + 0.5f, position.z }, velocity);

	bullets.push_back(std::move(newBullet));
	return true;
}

//当たったらシェイク掛ける(麻痺)
void Player::Numb(bool flag)
{
	if (flag) {
		shake->SetShakeStart(true);
	}
	else {
		shake->SetShakeStart(false);
	}
	//シェイクの最大値最小値などを入れる
	shake->ShakePos(shakePos.x, 1, -1, 10, 20);
	shake->ShakePos(shakePos.z, 1, -1, 10, 20);
	//フラグがfalseだったらシェイクをともる
	if (!shake->GetShakeStart()) {
		shakePos = { 0.0f, 0.0f, 0.0f };
	}
	//ポジションにシェイクの動きを足す
	position.x += shakePos.x;
	position.z += shakePos.z;
	Object3d::SetPosition(position);
}


void Player::OnCollision(int i)
{
	life -= i;
	CreateParticle();
}

//パーティクルの生成
void Player::CreateParticle()
{
	for (int j = 0; j < 100; j++) {
		DirectX::XMFLOAT3 pos = Object3d::GetPosition();
		//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
		const float md_vel = 0.20f;
		DirectX::XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		DirectX::XMFLOAT3 acc{};
		const float rnd_acc = 0.005f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
		//追加
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);

	}
}

void Player::Draw()
{
	Object3d::Draw();
	for (std::unique_ptr<PlayerBullet>& bul : bullets) {
		bul->Draw();
	}
}

XMVECTOR Player::GetWorldPosition()
{
	XMVECTOR worldPos{};

	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}

//チュートリアル用のアップデート
void Player::TutorialUpdate()
{
	//弾のフラグがfalseになったら削除する
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return !bullet->GetAlive();
		});
	move();
	//フェーズが位置にならないとjump出来ない
	if (operatePhase >= 1) {
		jump();
	}
	Input* input = Input::GetInstance();
	if (operatePhase >= 2) {
		if (input->TriggerMouseLeft()) {
			Shoot();
			shotFlag = true;
		}
		if (shotFlag) {
			operatePhase = 3;
		}
	}
	for (std::unique_ptr<PlayerBullet>& bul : bullets) {
		bul->Update();
	}
	Object3d::Update();
}

//チュートリアル用の描画関数
void Player::TutorialDraw(bool flag)
{
	Object3d::Draw();
	if (!flag) {
		for (std::unique_ptr<PlayerBullet>& bul : bullets) {
			bul->Draw();
		}
	}

}
