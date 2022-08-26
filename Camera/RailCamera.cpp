#include "RailCamera.h"

void RailCamera::Initialize(const Vector3 pos, const Vector3 rad){
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rad;
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	input_ = Input::GetInstance();

}

void RailCamera::Update(){
	//キャラクターの移動ベクトル
	Vector3 rotat = { 0, 0, 0 };
	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.1f;

	//キャラクターの移動ベクトル
	Vector3 cameraMove = { 0, 0, 0 };
	//キャラクターの移動の速さ
	const float cameraSpeed = 0.005f;


	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		cameraMove = { 0, kCharacterSpeed, 0 };
	}
	else if (input_->PushKey(DIK_S)) {
		cameraMove = { 0, -kCharacterSpeed, 0 };
	}
	if (input_->PushKey(DIK_A)) {
		cameraMove = { -kCharacterSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_D)) {
		cameraMove = { kCharacterSpeed, 0, 0 };
	}
	if (input_->PushKey(DIK_E)) {
		cameraMove = { 0, 0, -kCharacterSpeed };
	}
	else if (input_->PushKey(DIK_R)) {
		cameraMove = { 0, 0, kCharacterSpeed };
	}

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_H)) {
		rotat = { 0, cameraSpeed, 0 };
	}
	else if (input_->PushKey(DIK_F)) {
		rotat = { 0, -cameraSpeed, 0 };
	}
	if (input_->PushKey(DIK_T)) {
		rotat = { -cameraSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_G)) {
		rotat = { cameraSpeed, 0, 0 };
	}
	if (input_->PushKey(DIK_U)) {
		rotat = { 0, 0, -cameraSpeed };
	}
	else if (input_->PushKey(DIK_Y)) {
		rotat = { 0, 0, cameraSpeed };
	}

	worldTransform_.rotation_ += rotat;
	worldTransform_.translation_ += cameraMove;

	//行列更新
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();

	viewProjection_.eye = worldTransform_.translation_;
	
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = AffinTrans::MatVector(worldTransform_.matWorld_, forward);
	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;
	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	viewProjection_.up = AffinTrans::MatVector(worldTransform_.matWorld_,up);
	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();


}

ViewProjection RailCamera::GetViewProjection(){
	return viewProjection_;
}

WorldTransform* RailCamera::GetworldTransform(){
	return &worldTransform_;
}

