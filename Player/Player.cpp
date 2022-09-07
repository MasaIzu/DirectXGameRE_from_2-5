#include "Player.h"
#include "WinApp.h"
#include "MathUtility.h"
using namespace MathUtility;

void Player::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);
	model_ = model;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 2, -2, 3 };

	//3Dモデルの作成
	modelBullet_ = Model::CreateFromOBJ("Bullet", true);
	playerModel_ = Model::CreateFromOBJ("juu", true);
	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	textureReticle = TextureManager::Load("aim.png");
	//スプライト生成
	sprite2DReticle_.reset(Sprite::
		Create(textureReticle, Vector2(WinApp::GetInstance()->kWindowWidth / 2, WinApp::GetInstance()->kWindowHeight / 2 + 12),
			Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	textureReticle_FULL = TextureManager::Load("HPbar/HPfull.png");
	textureReticle_1 = TextureManager::Load("HPbar/HP_1.png");
	textureReticle_2 = TextureManager::Load("HPbar/HP_2.png");
	textureReticle_3 = TextureManager::Load("HPbar/HP_3.png");
	textureReticle_4 = TextureManager::Load("HPbar/HP_4.png");
	textureReticle_5 = TextureManager::Load("HPbar/HP_5.png");
	textureReticle_6 = TextureManager::Load("HPbar/HP_6.png");
	textureReticle_7 = TextureManager::Load("HPbar/HP_ALL.png");


	texture2dame = TextureManager::Load("2dame.png");
	texture4dame = TextureManager::Load("4dame.png");


	spriteHP_FULL.reset(Sprite::
		Create(textureReticle_FULL, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_1.reset(Sprite::
		Create(textureReticle_1, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_2.reset(Sprite::
		Create(textureReticle_2, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_3.reset(Sprite::
		Create(textureReticle_3, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_4.reset(Sprite::
		Create(textureReticle_4, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_5.reset(Sprite::
		Create(textureReticle_5, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_6.reset(Sprite::
		Create(textureReticle_6, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_7.reset(Sprite::
		Create(textureReticle_7, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	DrawHpSelect = 0;
	playerHp = 7;
	isShake = 0;
	shakeTime = 10;

	audio = Audio::GetInstance();
	shotHandol = audio->LoadWave("shot.mp3");
	damegeHandol = audio->LoadWave("dameji.mp3");

	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();
}

void Player::Move() {

	////キャラクターの移動ベクトル
	//Vector3 move = { 0, 0, 0 };
	////キャラクターの移動の速さ
	//const float kCharacterSpeed = 0.1f;

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_UP)) {
	//	move = { 0, kCharacterSpeed, 0 };
	//}
	//else if (input_->PushKey(DIK_DOWN)) {
	//	move = { 0, -kCharacterSpeed, 0 };
	//}
	//if (input_->PushKey(DIK_LEFT)) {
	//	move = { -kCharacterSpeed, 0, 0 };
	//}
	//else if (input_->PushKey(DIK_RIGHT)) {
	//	move = { kCharacterSpeed, 0, 0 };
	//}

	////ゲームパッドの状態を知る変数(XINPUT)
	//XINPUT_STATE joyState;

	////ジョイスティック状態
	//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
	//	move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	//}


	//worldTransform_.translation_ += move;

	//行列更新
	AffinTrans::affin(worldTransform_);
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	worldTransform_.TransferMatrix();
}

void Player::ReticleUpdate(const ViewProjection& viewProjection) {


#pragma region //自機のワールド座標から3Dレティクルのワールド座標を計算

	////自機から3Dレティクルへの距離
	//const float kDistancePlayerTo3DReticle = 60.0f;
	////自機から3Dレティクルへのオフセット(Z+向き)
	//Vector3 offset = { 0,0,1.0f };
	////自機のワールド座標の回転を反映
	//offset = AffinTrans::MatVector(worldTransform_.matWorld_, offset);
	////ベクトルの長さを整える
	//offset = Vector3Normalize(offset) * kDistancePlayerTo3DReticle;
	////3Dレティクルの座標を設定
	//worldTransform3DReticle_.translation_ = AffinTrans::AddVector3(offset, AffinTrans::GetWorldTransform(worldTransform_.matWorld_));
	//worldTransform3DReticle_.translation_ += Vector3(0, 1, 0);
	////行列更新
	//AffinTrans::affin(worldTransform3DReticle_);
	//worldTransform3DReticle_.TransferMatrix();

#pragma endregion

#pragma region //3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算

	//Vector3 positionReticle = AffinTrans::GetWorldTransform(worldTransform3DReticle_.matWorld_);

	//Vector2 windowWH = Vector2(WinApp::GetInstance()->kWindowWidth, WinApp::GetInstance()->kWindowHeight);

	////ビューポート行列
	//Matrix4 Viewport =
	//{ windowWH.x / 2,0,0,0,
	//0,-windowWH.y / 2,0,0,
	//0,0,1,0,
	//windowWH.x / 2, windowWH.y / 2,0,1 };

	////ビュー行列とプロジェクション行列、ビューポート行列を合成する
	//Matrix4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * Viewport;

	////ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	//positionReticle = AffinTrans::DivVecMat(positionReticle, matViewProjectionViewport);

	////スプライトのレティクルに座標設定
	//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

#pragma endregion

#pragma region //スクリーン座標から2Dレティクルのワールド座標を計算

	//Vector2 windowWH = Vector2(WinApp::GetInstance()->kWindowWidth / 2, WinApp::GetInstance()->kWindowHeight / 2);

	//{
	//	//POINT mousePosition;
	//	////マウス座標(スクリーン座標)を取得する
	//	//GetCursorPos(&mousePosition);
	//	////クライアントエリア座標に変換する
	//	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//	//ScreenToClient(hwnd, &mousePosition);
	//	//int xPos_absolute, yPos_absolute;
	//	//int xPos = windowWH.x;  //移動させたいｘ座標（ウィンドウ内の相対座標）
	//	//int yPos = windowWH.y; //移動させたいｙ座標（ウィンドウ内の相対座標）
	//	//WINDOWINFO windowInfo;
	//	////ウィンドウの位置を取得
	//	//windowInfo.cbSize = sizeof(WINDOWINFO);
	//	//GetWindowInfo(hwnd, &windowInfo);
	//	////マウスの移動先の絶対座標（モニター左上からの座標）
	//	//xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	//	//yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス
	//	////マウスの移動量を取得
	//	//MouseMove = Vector2(0, 0);
	//	//MouseMove = (Vector2(mousePosition.y,mousePosition.x ) - Vector2(windowWH.y, windowWH.x));//座標軸で回転している関係でこうなる(XとYが入れ替え)
	//	//mouseMoved += Vector2(MouseMove.x, MouseMove.y);
	//	//MouseMove = MouseMove / 100;
	//	//SetCursorPos(xPos_absolute, yPos_absolute);
	//}


	//POINT mousePosition;
	//GetCursorPos(&mousePosition);

	////クライアントエリア座標に変換する
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);
	////ビューポート行列 (少し上にある)
	//Matrix4 Viewport =
	//{ windowWH.x,0,0,0,
	//0,-windowWH.y,0,0,
	//0,0,1,0,
	//windowWH.x, windowWH.y,0,1 };

	////ビュープロジェクションビューポート合成
	//Matrix4 matVPV = viewProjection.matView * viewProjection.matProjection * Viewport;

	////合成行列の逆行列を計算する
	//Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);
	////スクリーン座標
	//Vector3 posNear = Vector3(mousePosition.x + mouseMoved.x, mousePosition.y + mouseMoved.y, 0);
	//Vector3 posFar = Vector3(mousePosition.x + mouseMoved.x, mousePosition.y + mouseMoved.y, 1);

	////スクリーン座標系からワールド座標系へ
	//posNear = AffinTrans::DivVecMat(posNear, matInverseVPV);
	//posFar = AffinTrans::DivVecMat(posFar, matInverseVPV);

	////マウスレイの方向
	//Vector3 mouseDirection = posFar - posNear;
	//mouseDirection = Vector3Normalize(mouseDirection);
	////カメラから照準オブジェクトの距離
	//const float kDistanceTestObject = 80.0f;
	//worldTransform3DReticle_.translation_ = AffinTrans::AddVector3(posNear, mouseDirection * kDistanceTestObject);

	//
	////行列更新
	//AffinTrans::affin(worldTransform3DReticle_);
	//worldTransform3DReticle_.TransferMatrix();

	//debugText_->SetPos(50, 70);
	//debugText_->Printf(
	//	"target:(%f,%f,%f)", mousePosition.x + mouseMoved.x, mousePosition.y + mouseMoved.y,
	//	mousePosition.y + mouseMoved.y);

#pragma endregion

#pragma region //ジョイスティックでのレティクル

	////スプライトの現在座標を取得
	//Vector2 spritePosition = sprite2DReticle_->GetPosition();

	//XINPUT_STATE joyState;

	////ジョイスティック状態取得
	//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
	//	spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
	//	//スプライトの座標変更を反映
	//	sprite2DReticle_->SetPosition(spritePosition);
	//}

#pragma endregion


}

void Player::Update(const ViewProjection& viewProjection) {

	Move();
	//弾発射処理
	//発射タイマーカウントダウン
	BulletTimer--;
	//レティクル
	ReticleUpdate(viewProjection);

	if (isShake == 1) {
		shakeTime--;
		if (shakeTime < 0) {
			isShake = 0;
		}
	}

	//時間に達したら
	if (BulletTimer <= 0) {
		Attack(viewProjection);				//弾を発射
		BulletTimer = 20;	//発射タイマーを初期化
	}

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update(viewProjection);
	}
}

void Player::Draw(ViewProjection viewProjection_) {

	playerModel_->Draw(worldTransform_, viewProjection_);
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}


void Player::Attack(const ViewProjection& viewProjection_) {
	if (input_->IsTriggerMouse(0) || input_->IsPressMouse(0)) {

		audio->PlayWave(shotHandol, false, 0.4f);

		//弾の速度
		const float kBulletSpeed = 4.0f;
		Vector3 velocity(0, 0, kBulletSpeed);


		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = bVelocity(velocity, worldTransform_);

		//自機から照準オブジェクトへのベクトル
		velocity = viewProjection_.target - viewProjection_.eye;
		velocity = Vector3Normalize(velocity) * kBulletSpeed;

		//弾を生成し初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->worldSet(&worldTransform_);
		//弾をカメラ位置にセット
		newBullet->Initialize(modelBullet_, AffinTrans::GetWorldTransform(worldTransform_.parent_->matWorld_), velocity, texture2dame, texture4dame);

		//弾の登録
		bullets_.push_back(std::move(newBullet));
	}
#pragma region //ジョイスティックでの弾出し
	//XINPUT_STATE joyState;
	////ゲームパッド未接続ならば何もせず抜ける
	//if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	return;
	//}
	////Rトリガーを押していたら
	//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
	//	//弾の速度
	//	const float kBulletSpeed = 1.0f;
	//	Vector3 velocity(0, 0, kBulletSpeed);

	//	//速度ベクトルを自機の向きに合わせて回転させる
	//	velocity = bVelocity(velocity, worldTransform_);

	//	//自機から照準オブジェクトへのベクトル
	//	velocity = AffinTrans::GetWorldTransform(worldTransform3DReticle_.matWorld_) - AffinTrans::GetWorldTransform(worldTransform_.matWorld_);
	//	velocity = Vector3Normalize(velocity) * kBulletSpeed;

	//	//弾を生成し初期化
	//	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//	newBullet->worldSet(&worldTransform_);
	//	newBullet->Initialize(modelBullet_, AffinTrans::GetWorldTransform(worldTransform_.matWorld_), velocity);

	//	//弾の登録
	//	bullets_.push_back(std::move(newBullet));
	//}
#pragma endregion
}

void Player::DrawSprite() {
	sprite2DReticle_->Draw();

	if (DrawHpSelect == 0) {
		if (playerHp == 7) {
			spriteHP_FULL->Draw();
		}
		else if (playerHp == 6) {
			spriteHP_1->Draw();
		}
		else if (playerHp == 5) {
			spriteHP_2->Draw();
		}
		else if (playerHp == 4) {
			spriteHP_3->Draw();
		}
		else if (playerHp == 3) {
			spriteHP_4->Draw();
		}
		else if (playerHp == 2) {
			spriteHP_5->Draw();
		}
		else if (playerHp == 1) {
			spriteHP_6->Draw();
		}
		else {
			spriteHP_7->Draw();
		}
	}

}

void Player::OnCollision() {
	audio->PlayWave(damegeHandol, false, 0.4f);
	playerHp--;
	isShake = 1;
	shakeTime = 30;
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

int Player::GetShake() {
	return isShake;
}

int Player::GetPlayerHp() {
	return playerHp;
}

void Player::worldReticleSet(WorldTransform* worldTransform) {

	worldTransform_.parent_ = worldTransform;

}


void Player::SetTarget(Vector3 Target) {

	target_ = Target;

}

void Player::setDrawSelect(int x) {
	DrawHpSelect = x;
}