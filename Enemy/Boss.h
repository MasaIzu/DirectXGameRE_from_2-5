#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "DebugText.h"
#include "affin.h"
#include "EnemyBullet.h"

//自機クラスの前方宣言
class Player;

//GameSceneの前方宣言(苦肉の策)
class GameScene;

class Boss {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,Vector3 vector3, Model* handmodel, Model* minModel);

	/// <summary>
	/// 更新
	/// </summary>
	void Move();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector3 worldTransform);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	void Attack(const Vector3 worldTransform);

	/// <summary>
	/// 接近
	/// </summary>
	void Approach();

	/// <summary>
	/// 離脱
	/// </summary>
	void Leave();

	/// <summary>
	/// Up
	/// </summary>
	void Up();

	/// <summary>
	/// Near
	/// </summary>
	void Near();

	/// <summary>
	/// 戻る
	/// </summary>
	void Back();

	/// <summary>
	/// 衝突時に呼び出すコールバック関数
	/// </summary>
	void OnCollisionBoss();
	void OnCollisionRight();
	void OnCollisionLeft();



	bool IsDead() const { return isDead_; }

	enum class Phase {
		Approach,//接近する
		Leave,//離脱する
		Up,//上昇
		Near,//近づく
		Back,//離れる
	};

	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);


	//発射間隔
	static const int kFireInterval = 60;

	void  SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPositionBoss();
	Vector3 GetWorldPositionRight();
	Vector3 GetWorldPositionLeft();

	//ゲームシーンSetter
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//打つなら0打たないなら1
	void attackSet(int x);

	int GetBossHp();

private:

	//ワールド変換データ
	WorldTransform worldTransformBoss_;
	WorldTransform worldTransformBossRightHand_;
	WorldTransform worldTransformBossLeftHand_;
	//モデル
	Model* model_ = nullptr;
	Model* minModel_ = nullptr;
	Model* handModel_ = nullptr;
	DebugText* debugText_ = nullptr;
	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//テクスチャハンドル
	uint32_t enemyTextureHandle_ = 0;
	//テクスチャハンドル
	uint32_t enemy50TextureHandle_ = 0;

	//自キャラ
	Player* player_ = nullptr;

	//enemyHp
	int bossHP = 0;

	//フラグ
	int back = 0;
	//キャラクターの移動の速さ
	const float kEnemyCharacterSpeed = 0.2f;
	//キャラクターの移動ベクトル
	Vector3 move = { kEnemyCharacterSpeed, 0, 0 };
	Vector3 rot = { 0,0.01f,0 };
	//フェーズ
	Phase phase_ = Phase::Approach;
	//キャラクターのフェーズ移動の速さ
	const float kEnemyPhaseCharacterSpeed = 0.1f;
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;
	//デスフラグ
	bool isDead_ = false;

	int BulletTimer = 0;

	int phaseTimer = 0;

	float KeepY = 0;
	Vector3 keep;
	Vector3 MovePhase{ 0,0,0 };
	Vector3 BackPhase{ 0,0,0 };
	int MoveTimer;

	int enemyAttackFlag = 0;

	int upDown = 0;
	int a = 0;
};