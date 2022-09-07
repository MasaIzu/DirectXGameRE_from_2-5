#include "Ground.h"

void Ground::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = Vector3(0, -0.25f, 0);
	worldTransform_.scale_ = Vector3(600, 100, 600);
	worldTransform_.matWorld_ *= AffinTrans::Move(worldTransform_.translation_);
	worldTransform_.matWorld_ *= AffinTrans::Scale(worldTransform_.scale_);
	//行列の転送
	worldTransform_.TransferMatrix();
}

void Ground::Update() {

}

void Ground::Draw(ViewProjection viewProjection_) {
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_);
}
