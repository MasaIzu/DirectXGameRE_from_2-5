#include "Select.h"

void Select::Initialize(Model* GroundModel, Model* modelDome) {
	//NULLポインタチェック
	assert(GroundModel);
	GroundModel_ = GroundModel;
	//ワールド変換の初期化
	worldGroundTransform_.Initialize();

	worldGroundTransform_.translation_ = Vector3(0, -0.25f, 0);
	worldGroundTransform_.scale_ = Vector3(600, 100, 600);
	worldGroundTransform_.matWorld_ *= AffinTrans::Move(worldGroundTransform_.translation_);
	worldGroundTransform_.matWorld_ *= AffinTrans::Scale(worldGroundTransform_.scale_);

	//NULLポインタチェック
	assert(modelDome);
	modelDome_ = modelDome;
	//ワールド変換の初期化
	worldDomeTransform_.Initialize();
	worldDomeTransform_.scale_ = Vector3(600, 600, 600);
	worldDomeTransform_.matWorld_ *= AffinTrans::Scale(worldDomeTransform_.scale_);

	taitolModel_ = Model::Create();
	textureHandle_ = TextureManager::Load("taitol_Maverick_hanten.png");
	startTrans.Initialize();
	startTrans.translation_ = Vector3{ 0,-16,-40 };
	startTrans.scale_ = Vector3(6, 3, 1);
	startTrans.rotation_ = Vector3(-0.2, 0, 0);
	startTrans.matWorld_ *= AffinTrans::Scale(startTrans.scale_);

	textureHandle2_ = TextureManager::Load("setumeihanten.png");
	startTrans2.Initialize();
	startTrans2.translation_ = Vector3{ 14,-16,-43 };
	startTrans2.scale_ = Vector3(6, 3, 1);
	startTrans2.rotation_ = Vector3(-0.2, 0.9, 0);
	startTrans2.matWorld_ *= AffinTrans::Scale(startTrans2.scale_);




	//行列の転送
	AffinTrans::affin(startTrans);
	AffinTrans::affin(startTrans2);
	worldGroundTransform_.TransferMatrix();
	worldDomeTransform_.TransferMatrix();
	startTrans.TransferMatrix();
	startTrans2.TransferMatrix();
}

void Select::Update() {

}

void Select::Draw(ViewProjection viewProjection_) {
	//3Dモデル描画
	GroundModel_->Draw(worldGroundTransform_, viewProjection_);
	modelDome_->Draw(worldDomeTransform_, viewProjection_);
	taitolModel_->Draw(startTrans, viewProjection_, textureHandle_);
	taitolModel_->Draw(startTrans2, viewProjection_, textureHandle2_);
}
