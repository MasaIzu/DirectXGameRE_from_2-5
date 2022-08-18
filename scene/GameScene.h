#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin.h"
#include <Player/Player.h>
#include <Enemy/Enemy.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
		/// <summary>
		/// コンストクラタ
		/// </summary>
	GameScene();

public:


	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデル
	Model* model_ = nullptr;


	//ビュープロジェクション
	ViewProjection viewProjection_;
	////デバッグカメラ
	//DebugCamera* debugCamera_ = nullptr;
	//カメラ上方向
	float viewAngle = 0.0f;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;

	Vector3 vector3(float x, float y, float z);
	Vector4 vector4(int x, int y, int z, int w);

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};