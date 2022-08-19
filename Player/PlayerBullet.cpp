#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Black.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

}

void PlayerBullet::Update() {

	//座標を移動させる(1フレーム文の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	//時間でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision(){
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition(){

	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}