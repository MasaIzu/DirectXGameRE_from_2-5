#include "Player.h"
#include "WinApp.h"
using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = { -7,-5,20 };

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("aim.png");
	//スプライト生成
	sprite2DReticle_.reset(Sprite::Create(textureReticle, Vector2(360, 250), Vector4(1, 1, 1, 1),Vector2(0.5, 0.5)));
	
	

}

void Player::Move() {

	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };
	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.1f;

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

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

	worldTransform_.translation_ += move;

	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	const float kChestRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= kChestRotSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += kChestRotSpeed;
	}

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	worldTransform_.TransferMatrix();

	debugText_->SetPos(100, 20);
	debugText_->Printf(
		"PlayerVec : %f,%f,%f", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

#pragma region //自機のワールド座標から3Dレティクルのワールド座標を計算

	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 10.0f;
	//自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0,0,1.0f };
	//自機のワールド座標の回転を反映
	offset = AffinTrans::MatVector(worldTransform_.matWorld_, offset);
	//ベクトルの長さを整える
	offset = Vector3Normalize(offset) * kDistancePlayerTo3DReticle;
	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = AffinTrans::AddVector3(offset,AffinTrans::GetWorldTransform(worldTransform_.matWorld_));

	//行列更新
	AffinTrans::affin(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();

#pragma endregion

	//弾発射処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	//sprite2DReticle_->SetPosition(Vector2{ mousePosition.x, mousePosition.y });

}

void Player::Update() {

	Move();

}

void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::DrawUI(){

	sprite2DReticle_->Draw();

}


void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = bVelocity(velocity, worldTransform_);

		//自機から照準オブジェクトへのベクトル
		velocity = AffinTrans::GetWorldTransform(worldTransform3DReticle_.matWorld_) - AffinTrans::GetWorldTransform(worldTransform_.matWorld_);
		velocity = Vector3Normalize(velocity) * kBulletSpeed;

		//弾を生成し初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, AffinTrans::GetWorldTransform(worldTransform_.matWorld_), velocity);

		//弾の登録
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::OnCollision() {

}

Vector3 Player::bVelocity(Vector3 velocity, WorldTransform& worldTransform) {

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

Vector3 Player::GetWorldPosition() {

	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::worldSet(WorldTransform* worldTransform) {

	worldTransform_.parent_ = worldTransform;

}
