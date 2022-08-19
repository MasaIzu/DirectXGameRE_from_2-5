#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "DebugText.h"
#include "affin.h"
#include "EnemyBullet.h"

class Enemy {

public:
	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// <summary>
	void Move();

	/// <summary>
	/// 更新
	/// <summary>
	void Update();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw(ViewProjection& viewProjection_);

	void Attack();

	/// <summary>
	/// 接近
	/// <summary>
	void ApproachInitialize();
	void Approach();

	/// <summary>
	/// 離脱
	/// <summary>
	void Leave();

	enum class Phase {
		Approach,//接近する
		Leave,//離脱する
	};

	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	void Fire();
	void BulletClean();

	//発射間隔
	static const int kFireInterval = 60;

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//フラグ
	int back = 0;
	//キャラクターの移動の速さ
	const float kEnemyCharacterSpeed = 0.05f;
	//キャラクターの移動ベクトル
	Vector3 move = { kEnemyCharacterSpeed, 0, 0 };

	//フェーズ
	Phase phase_ = Phase::Approach;
	//キャラクターのフェーズ移動の速さ
	const float kEnemyPhaseCharacterSpeed = 0.01f;

	//弾
	std::list<std::unique_ptr<EnemyBullet>>bullets_;

	int BulletTimer = 0;
};