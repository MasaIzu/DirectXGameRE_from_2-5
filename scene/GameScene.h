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
#include "Skydome/Skydome.h"
#include "Camera/RailCamera.h"
#include <sstream>


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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加
	/// </summary>
	/// <param name = "enemyBullet">敵弾</param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 敵発生関数
	/// </summary>
	void MakeEnemy(Vector3 EnemyPos);

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return enemyBullets_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//レティクルのテクスチャハンドル
	uint32_t ReticleTextureHandle_ = 0;
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
	int playerRadius = 1;
	//自弾
	int playerBulletRadius = 1;
	//敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;
	int enemyRadius = 1;
	//敵弾
	//弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
	int enemyBulletRadius = 1;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
	//Skydome
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//待機中フラグ
	bool isStand_ = false;

	//待機タイマー
	int standTime_ = 0;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};