#include"Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ += Vector3(0, 0, 10);

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();
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

	debugText_->SetPos(50, 180);
	debugText_->Printf(
		"translation : %f,%f,%f", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

}

void Enemy::Update() {

	Move();

}

void Enemy::Draw(ViewProjection& viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}