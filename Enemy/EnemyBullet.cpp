#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	//NULLチェック
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

void EnemyBullet::Update() {
	
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

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
