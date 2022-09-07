#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle_, uint32_t textureHandle) {
	assert(model);

	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	worldTransform_.translation_ += Vector3(0, -2, 0);
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	texture2Handle_ = textureHandle_;
	texture4Handle_ = textureHandle;

	isHit = 0;
	//行列更新
	AffinTrans::affin(worldTransform_);
	//worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Update(const ViewProjection& viewProjection) {

	//座標を移動させる(1フレーム文の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;


	//worldTransform_.rotation_ += Vector3(0.1f, 0.1f, 0.1f);

	//行列更新
	AffinTrans::affin(worldTransform_);
	worldTransform_.matWorld_ *= viewProjection.matView;
	worldTransform_.TransferMatrix();

	//時間でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {


	model_->Draw(worldTransform_, viewProjection);

	if (isHit == 1) {

	}
}

void PlayerBullet::OnCollision() {
	isHit = 1;
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition() {

	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::worldSet(WorldTransform* worldTransform) {

	worldTransform_.parent_ = worldTransform->parent_;

}

