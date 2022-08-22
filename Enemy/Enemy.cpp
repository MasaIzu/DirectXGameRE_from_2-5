#include"Enemy.h"
#include"Player/Player.h"
#include <cmath>
#include <GameScene.h>

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 vector3) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = vector3;

	//行列更新
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();

	ApproachInitialize();

}

void Enemy::Move() {

	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;


	//範囲を超えない処理
	if (worldTransform_.translation_.x <= -kMoveLimitX) {
		move = { kEnemyCharacterSpeed, 0, 0 };
	}
	if (worldTransform_.translation_.x >= kMoveLimitX) {
		move = { -kEnemyCharacterSpeed, 0, 0 };
	}

	worldTransform_.translation_ += move;
	//行列更新
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}


	debugText_->SetPos(400, 20);
	debugText_->Printf(
		"PlayerVec : %f,%f,%f", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Enemy::Update() {

	
	Move();

}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

}

void Enemy::ApproachInitialize() {

	//発射タイマーの初期化
	BulletTimer = kFireInterval;

}

void Enemy::Approach() {
	//発射タイマーカウントダウン
	BulletTimer--;
	//時間に達したら
	if (BulletTimer <= 0) {
		Attack();				//弾を発射
		BulletTimer = 60;	//発射タイマーを初期化
	}

	//移動ベクトル
	worldTransform_.translation_ += Vector3(0, 0, -kEnemyPhaseCharacterSpeed);
	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < -20.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	//発射タイマーカウントダウン
	BulletTimer--;
	//時間に達したら
	if (BulletTimer <= 0) {
		Attack();				//弾を発射
		BulletTimer = 60;	//発射タイマーを初期化
	}
	//移動(ベクトルを加算)
	worldTransform_.translation_ += Vector3(0, 0, +kEnemyPhaseCharacterSpeed);
	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z > 20.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::OnCollision(){
	isDead_ = true;
}

void Enemy::Attack() {

	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = bVelocity(velocity, worldTransform_);

	Vector3 PlayerVec = player_->GetWorldPosition();
	Vector3 EnemyVec = GetWorldPosition();

	Vector3 A_BVec = Vector3(PlayerVec.x - EnemyVec.x, PlayerVec.y - EnemyVec.y, PlayerVec.z - EnemyVec.z);
	float nomalize = sqrt(A_BVec.x * A_BVec.x + A_BVec.y * A_BVec.y + A_BVec.z * A_BVec.z) * 10;
	A_BVec = Vector3(A_BVec.x / nomalize, A_BVec.y / nomalize, A_BVec.z / nomalize);


	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, A_BVec);

	//弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
	////リストに登録する
	//enemyBullets_.push_back(std::move(enemyBullet));

	debugText_->SetPos(400, 20);
	debugText_->Printf(
		"PlayerVec : %f,%f,%f", PlayerVec.x,
		PlayerVec.y,
		PlayerVec.z);
	
}

Vector3 Enemy::bVelocity(Vector3& velocity, WorldTransform& worldTransform) {

	Vector3 result = { 0, 0, 0 };

	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];


	return result;
}


Vector3 Enemy::GetWorldPosition(){
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
