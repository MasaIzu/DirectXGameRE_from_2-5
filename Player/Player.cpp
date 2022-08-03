#include "Player.h"


void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	int i = 0; int ibuf;
	for (WorldTransform& worldTransform : worldTransforms_) {

		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		
		i++;
		if (i < 26) {
			// X,Y,Z軸の周りの平行移動を設定
			worldTransform.translation_ = Vector3(0 + 2 * i, 0, 0);
		}
		else {
			ibuf = i;
			i = i - 25;
			// X,Y,Z軸の周りの平行移動を設定
			worldTransform.translation_ = Vector3(0 + 2 * i, 5, 0);
			i = ibuf;
		}

		//合成の計算
		worldTransform.matWorld_.m[0][0] = 1.0f;
		worldTransform.matWorld_.m[1][1] = 1.0f;
		worldTransform.matWorld_.m[2][2] = 1.0f;
		worldTransform.matWorld_.m[3][3] = 1.0f;

#pragma region スケーリング

		//Matrix4 mat = MathUtility::Matrix4Identity();

		worldTransform.matWorld_ = AffinTrans::Initialize();
		worldTransform.matWorld_ *= AffinTrans::Scale(Vector3(worldTransform.scale_.x, worldTransform.scale_.y, worldTransform.scale_.z));
	}
}

void Player::Move() {

	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };
	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_UP)) {
		move = { 0, kCharacterSpeed, 0 };
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { 0, -kCharacterSpeed, 0 };
	}
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharacterSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharacterSpeed, 0, 0 };
	}

	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.translation_ += move;

		//行列更新
		AffinTrans::affin(worldTransform);

		worldTransform.TransferMatrix();

		const float kMoveLimitX = 35;
		const float kMoveLimitY = 18;


		const float kChestRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U)) {
			worldTransform.rotation_.y -= kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_I)) {
			worldTransform.rotation_.y += kChestRotSpeed;
		}
	}
	//弾発射処理
	Attack();

	//弾更新
	if (bullet_) {
		bullet_->Update();
	}

}

void Player::Update() {

	Move();

}

void Player::Draw(ViewProjection& viewProjection_) {
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}
	//弾更新
	if (bullet_) {
		bullet_->Draw(viewProjection_);
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
		//弾を生成し初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransforms_[0].translation_);

		//弾の登録
		bullet_ = newBullet;
	}

}