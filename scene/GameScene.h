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
#include "Ground.h"
#include "Reticle.h"
#include"Select.h"
#include "Enemy/Boss.h"


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
	void UpdateEnemyPopCommands_phase1();
	void UpdateEnemyPopCommands_phase2();
	void UpdateEnemyPopCommands_phase3();
	void UpdateEnemyPopCommands_phase4();

	//シーン切り替え
	enum class Scene {
		First,//タイトル
		Second,//チュートリアル
		Third,//ステージ2
		Four,//語りかけ
		Boss,//ボス
		Back,//ty&タイトルへ
		Loser,//エース
	};


	/// <summary>
	/// 敵発生関数
	/// </summary>
	void MakeEnemy(Vector3 EnemyPos);

	/// <summary>
	/// ステージごとの初期化
	/// </summary>
	void secondInitialize();

	/// <summary>
	/// 敵と敵の弾アプデ
	/// </summary>
	void enemyUpdate(int x);

	/// <summary>
	/// 敵の後始末
	/// </summary>
	void enemybay();

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return enemyBullets_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//テクスチャハンドル
	uint32_t enemyTextureHandle_ = 0;
	//テクスチャハンドル
	uint32_t enemy50TextureHandle_ = 0;
	//レティクルのテクスチャハンドル
	uint32_t ReticleTextureHandle_ = 0;
	//テクスチャハンドル
	uint32_t dame2Texture_ = 0;
	uint32_t dame4Texture_ = 0;
	//ワールド変換データ
	WorldTransform worldTransformDame_;

	// 3Dモデル
	Model* model_ = nullptr;
	Model* EnemyModel_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	////デバッグカメラ
	//DebugCamera* debugCamera_ = nullptr;
	//カメラ上方向
	float viewAngle = 0.0f;

	//シーン分け
	Scene scene_ = Scene::First;


	//シーン次行く
	WorldTransform nextScene;
	std::unique_ptr<Sprite> nextSprite;
	uint32_t nextTexture_ = 0;


	std::unique_ptr<Sprite> finSprite;
	uint32_t finTexture_ = 0;

	std::unique_ptr<Sprite> loserSprite;
	uint32_t loserTexture_ = 0;

	//自キャラ
	Player* player_ = nullptr;
	int playerRadius = 2;
	//自弾
	int playerBulletRadius = 2;

	//敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;
	int enemyRadius = 3;
	//敵弾
	//弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
	int enemyBulletRadius = 1;

	//Boss
	Boss* boss_ = nullptr;
	int bossRadius = 30;
	Model* BossModel_ = nullptr;
	Model* BossHandModel_ = nullptr;
	int bossHandRadius = 20;


	//倒した敵の数
	int DeadEnemys = 0;
	//敵発生コマンド
	std::stringstream enemyPopCommand_phase1;
	std::stringstream enemyPopCommand_phase2;
	std::stringstream enemyPopCommand_phase3;
	std::stringstream enemyPopCommand_phase4;

	//Skydome
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//Ground
	Ground* ground_ = nullptr;
	Model* modelGround_ = nullptr;

	//SelectGround
	Select* select_ = nullptr;
	Model* selectModelGround_ = nullptr;
	Model* modelDome_ = nullptr;

	//待機中フラグ
	bool isStand_ = false;

	//待機タイマー
	int standTime_ = 0;

	//シーン切り替えタイマー
	int sceneChange = 180;

	//描画用の切り替え
	int changeDraw = 0;
	
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};