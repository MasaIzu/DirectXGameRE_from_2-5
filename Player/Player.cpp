#include "Player.h"
#include "WinApp.h"
#include "MathUtility.h"
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

	//ゲームパッドの状態を知る変数(XINPUT)
	XINPUT_STATE joyState;

	//ジョイスティック状態
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
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


}

void Player::ReticleUpdate(const ViewProjection& viewProjection){

#pragma region //自機のワールド座標から3Dレティクルのワールド座標を計算

	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 30.0f;
	//自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0,0,1.0f };
	//自機のワールド座標の回転を反映
	offset = AffinTrans::MatVector(worldTransform_.matWorld_, offset);
	//ベクトルの長さを整える
	offset = Vector3Normalize(offset) * kDistancePlayerTo3DReticle;
	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = AffinTrans::AddVector3(offset, AffinTrans::GetWorldTransform(worldTransform_.matWorld_));

	//行列更新
	AffinTrans::affin(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();

#pragma endregion

#pragma region //3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算

	Vector3 positionReticle = AffinTrans::GetWorldTransform(worldTransform3DReticle_.matWorld_);

	Vector2 windowWH = Vector2(WinApp::GetInstance()->kWindowWidth, WinApp::GetInstance()->kWindowHeight);

	//ビューポート行列
	Matrix4 Viewport =
	{ windowWH.x / 2,0,0,0,
	0,-windowWH.y / 2,0,0,
	0,0,1,0,
	windowWH.x / 2, windowWH.y / 2,0,1 };

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * Viewport;

	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = AffinTrans::DivVecMat(positionReticle, matViewProjectionViewport);

	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

#pragma endregion

#pragma region //スクリーン座標から2Dレティクルのワールド座標を計算
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));

	//ビューポート行列 (少し上にある)
	/*Matrix4 Viewport =
	{ windowWH.x / 2,0,0,0,
	0,-windowWH.y / 2,0,0,
	0,0,1,0,
	windowWH.x / 2, windowWH.y / 2,0,1 };*/

	//ビュープロジェクションビューポート合成
	Matrix4 matVPV = viewProjection.matView * viewProjection.matProjection * Viewport;

	//合成行列の逆行列を計算する
	Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);
	//スクリーン座標
	Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
	Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

	//スクリーン座標系からワールド座標系へ
	posNear = AffinTrans::DivVecMat(posNear, matInverseVPV);
	posFar = AffinTrans::DivVecMat(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Vector3Normalize(mouseDirection);
	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 80.0f;
	worldTransform3DReticle_.translation_ = AffinTrans::AddVector3(posNear, mouseDirection * kDistanceTestObject);

	//行列更新
	AffinTrans::affin(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();

#pragma endregion

#pragma region //ジョイスティックでのレティクル

	//スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyState;

	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		//スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);
	}

#pragma endregion

}

void Player::Update(const ViewProjection& viewProjection) {

	Move();
	ReticleUpdate(viewProjection);
	//弾発射処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

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

	XINPUT_STATE joyState;
	//ゲームパッド未接続ならば何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	//Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
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
