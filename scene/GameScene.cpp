#include "GameScene.h"
#include "affin.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

const float PI = 3.141592f;

float RAD(float angle) {
	return angle * (PI / 180);
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
	//乱数エンジンを渡し、指定範囲からランダムな数値を得る
	float value = dist(engine);

	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, MathUtility::PI);

	//	for (WorldTransform& worldTransform : worldTransforms_) {
	//
	//		//ワールドトランスフォームの初期化
	//		worldTransform.Initialize();
	//
	//		// X,Y,Z 方向のスケーリングを設定
	//		worldTransform.scale_ = {1.0f, 1.0f, 1.0f};
	//		// X,Y,Z 軸回りの回転角を設定
	//		worldTransform.rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
	//		// X,Y,Z軸の周りの平行移動を設定
	//		worldTransform.translation_ = {dist(engine), dist(engine), dist(engine)};
	//
	//#pragma region スケーリング
	//
	//	Matrix4 mat = MathUtility::Matrix4Identity();
	//
	//	worldTransform.matWorld_ *= AffinTrans::Scale(
	//	  vector3(worldTransform.scale_.x, worldTransform.scale_.y, worldTransform.scale_.z));
	//
	//	
	//#pragma endregion
	//
	////#pragma region Rotation_X
	////	////X,Y,Z軸周りの回転角を設定
	////	// worldTransform_.rotation_ = { PI / 4,0.0f,0.0f };
	////	////Z軸回転行列を宣言
	////	// Matrix4 matRotX;
	////	// matRotX = {
	////	//	1.0f,0.0f,0.0f,0.0f,
	////	//	0.0f,cos(worldTransform_.rotation_.x),  sin(worldTransform_.rotation_.x),0.0f,
	////	//	0.0f,-sin(worldTransform_.rotation_.x), cos(worldTransform_.rotation_.x),0.0f,
	////	//	0.0f,0.0f,0.0f,1.0f
	////	// };
	////
	////	// Matrix4 matRotX2;
	////	// matRotX2 = {
	////	//	1.0f,0.0f,0.0f,0.0f,
	////	//	0.0f,1.0f,0.0f,0.0f,
	////	//	0.0f,0.0f,1.0f,0.0f,
	////	//	0.0f,0.0f,0.0f,1.0f
	////	// };
	////
	////	//worldTransform_.matWorld_ = AffinTrans::Initialize();
	////	worldTransform_.matWorld_ *= AffinTrans::Rotation(vector3(PI / 4, 0.0f, 0.0f), 1);
	////
	////	//行列の転送
	////	worldTransform_.TransferMatrix();
	////#pragma endregion
	////
	////#pragma region Rotation_Y
	////	//	//X,Y,Z軸周りの回転角を設定
	////	//	worldTransform_.rotation_ = { 0.0f,PI / 4,0.0f };
	////	//	//Y軸回転行列を宣言
	////	//	Matrix4 matRotY;
	////	//	matRotY = {
	////	//		cos(worldTransform_.rotation_.y),0.0f,-sin(worldTransform_.rotation_.y),0.0f,
	////	//		0.0f,1.0f,0.0f,0.0f,
	////	//		sin(worldTransform_.rotation_.y),0.0f, cos(worldTransform_.rotation_.y),0.0f,
	////	//		0.0f,0.0f,0.0f,1.0f
	////	//	};
	////	//
	////	//	Matrix4 matRotY2;
	////	//	matRotY2 = {
	////	//		1.0f,0.0f,0.0f,0.0f,
	////	//		0.0f,1.0f,0.0f,0.0f,
	////	//		0.0f,0.0f,1.0f,0.0f,
	////	//		0.0f,0.0f,0.0f,1.0f
	////	//	};
	////	//
	////	//worldTransform_.matWorld_ = AffinTrans::Initialize();
	////	worldTransform_.matWorld_ *= AffinTrans::Rotation(vector3(0.0f, PI / 4, 0.0f), 2);
	////
	////	//行列の転送
	////	worldTransform_.TransferMatrix();
	////#pragma endregion
	////
	////#pragma region Rotation_Z
	////	//	//X,Y,Z軸周りの回転角を設定
	////	//	worldTransform_.rotation_ = { 0.0f,0.0f,PI / 4 };
	////	//	//Z軸回転行列を宣言
	////	//	Matrix4 matRotZ;
	////	//	matRotZ = {
	////	//		cos(worldTransform_.rotation_.z),sin(worldTransform_.rotation_.z),0.0f,0.0f,
	////	//		-sin(worldTransform_.rotation_.z),cos(worldTransform_.rotation_.z), 0.0f,0.0f,
	////	//		0.0f,0.0f, 1.0f,0.0f,
	////	//		0.0f,0.0f,0.0f,1.0f
	////	//	};
	////	//
	////	//	Matrix4 matRotZ2;
	////	//	matRotZ2 = {
	////	//		1.0f,0.0f,0.0f,0.0f,
	////	//		0.0f,1.0f,0.0f,0.0f,
	////	//		0.0f,0.0f,1.0f,0.0f,
	////	//		0.0f,0.0f,0.0f,1.0f
	////	//	};
	////	//
	////	//worldTransform_.matWorld_ = AffinTrans::Initialize();
	////	worldTransform_.matWorld_ *= AffinTrans::Rotation(vector3(0.0f, 0.0f, PI / 4), 3);
	////
	////	//行列の転送
	////	worldTransform_.TransferMatrix();
	////#pragma endregion
	//
	//#pragma region Rotation_X_Y_Z
	//
	//	worldTransform.matWorld_ *= AffinTrans::Rotation(
	//	  vector3(worldTransform.rotation_.x, worldTransform.rotation_.y, worldTransform.rotation_.z),
	//	  6);
	//
	//	
	//#pragma endregion
	//
	//#pragma region move
	//	worldTransform.matWorld_ *= AffinTrans::Move(vector3(
	//	  worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z));
	//
	//	//行列の転送
	//	worldTransform.TransferMatrix();
	//	}

		//キャラクターの大元
	worldTransforms_[Partid::kRoot].Initialize();
	//子(脊椎)
	worldTransforms_[Partid::kSpine].translation_ = { 0, 4.5f, 0 };
	worldTransforms_[Partid::kSpine].parent_ = &worldTransforms_[Partid::kRoot];
	worldTransforms_[Partid::kSpine].Initialize();

	//キャラクターの脊椎
	worldTransforms_[Partid::kSpine].Initialize();
	//子(胸)
	worldTransforms_[Partid::kChest].translation_ = { 0, -4.5f, 0 };
	worldTransforms_[Partid::kChest].parent_ = &worldTransforms_[Partid::kSpine];
	worldTransforms_[Partid::kChest].Initialize();

	//キャラクターの胸
	worldTransforms_[Partid::kChest].Initialize();
	//子(頭)
	worldTransforms_[Partid::kHead].translation_ = { 0, -4.5f, 0 };
	worldTransforms_[Partid::kHead].parent_ = &worldTransforms_[Partid::kChest];
	worldTransforms_[Partid::kHead].Initialize();

	//キャラクターの胸
	worldTransforms_[Partid::kChest].Initialize();
	//子(左腕)
	worldTransforms_[Partid::kArmL].translation_ = { -5, -4.5f, 0 };
	worldTransforms_[Partid::kArmL].parent_ = &worldTransforms_[Partid::kChest];
	worldTransforms_[Partid::kArmL].Initialize();

	//キャラクターの胸
	worldTransforms_[Partid::kChest].Initialize();
	//子(右腕)
	worldTransforms_[Partid::kArmR].translation_ = { 5, -4.5f, 0 };
	worldTransforms_[Partid::kArmR].parent_ = &worldTransforms_[Partid::kChest];
	worldTransforms_[Partid::kArmR].Initialize();

	//下半身

	//キャラクターの脊椎
	worldTransforms_[Partid::kSpine].Initialize();
	//子(ケツ)
	worldTransforms_[Partid::kHip].translation_ = { 0, -14.5f, 0 };
	worldTransforms_[Partid::kHip].parent_ = &worldTransforms_[Partid::kSpine];
	worldTransforms_[Partid::kHip].Initialize();

	//キャラクターのケツ
	worldTransforms_[Partid::kHip].Initialize();
	//子(左足)
	worldTransforms_[Partid::kLegL].translation_ = { -5, -4.5f, 0 };
	worldTransforms_[Partid::kLegL].parent_ = &worldTransforms_[Partid::kHip];
	worldTransforms_[Partid::kLegL].Initialize();

	//キャラクターの脊椎
	worldTransforms_[Partid::kHip].Initialize();
	//子(右足)
	worldTransforms_[Partid::kLegR].translation_ = { 5, -4.5f, 0 };
	worldTransforms_[Partid::kLegR].parent_ = &worldTransforms_[Partid::kHip];
	worldTransforms_[Partid::kLegR].Initialize();




	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

#pragma endregion
}

void GameScene::Update() {
	debugCamera_->Update();

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };
	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharacterSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharacterSpeed, 0, 0 };
	}

	//注視点(ベクトルの加算)
	worldTransforms_[Partid::kRoot].translation_.x += move.x;
	worldTransforms_[Partid::kRoot].translation_.y += move.y;
	/*AffinTrans::affin(worldTransforms_[0]);
	worldTransforms_[0].TransferMatrix();*/


	for (int i = 0; i < kNumPartid; i++) {
		AffinTrans::affin(worldTransforms_[i]);
		if (i != 0) {
			worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
		}
		worldTransforms_[i].TransferMatrix();
	}

	//上半身の回転処理
	//上半身の回転の速さ[ラジアン/frame]
	const float kChestRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_U)) {
		worldTransforms_[Partid::kChest].rotation_.y -= kChestRotSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransforms_[Partid::kChest].rotation_.y += kChestRotSpeed;
	}

	//下半身の回転処理
	//下半身の回転の速さ[ラジアン/frame]
	const float kHipRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_J)) {
		worldTransforms_[Partid::kHip].rotation_.y -= kHipRotSpeed;
	}
	else if (input_->PushKey(DIK_K)) {
		worldTransforms_[Partid::kHip].rotation_.y += kHipRotSpeed;
	}

	//デバッグ用表示
	/*debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)",
	   viewProjection_.target.x,
	   viewProjection_.target.y,
	   viewProjection_.target.z );
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "target:(%f,%f,%f)",
		viewProjection_.up.x,
		viewProjection_.up.y,
	  viewProjection_.up.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf("forAngleY(Degree):%f", RAD(viewProjection_.fovAngleY)
	);
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f",viewProjection_.nearZ);*/

	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"translation[Partid::kRoot] : %f,%f,%f", worldTransforms_[Partid::kRoot].translation_.x,
		worldTransforms_[Partid::kRoot].translation_.y,
		worldTransforms_[Partid::kRoot].translation_.z);

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/*for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}*/
	for (int i = 0; i < kNumPartid; i++) {
		if (i <= 1) {
			continue;
		}
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}
	/*model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);*/

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

Vector3 GameScene::vector3(float x, float y, float z) { return Vector3(x, y, z); }

Vector4 GameScene::vector4(int x, int y, int z, int w) { return Vector4(x, y, z, w); }