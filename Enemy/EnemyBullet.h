#pragma once
#include "Model.h"
#include "math/Vector3.h"
#include <Model.h>
#include "ViewProjection.h"
#include <WorldTransform.h>
#include <cassert>
#include "affin.h"

class EnemyBullet {

public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突時に呼び出すコールバック関数
	/// <summary>
	void OnCollision();

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();
private:

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;
	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

};