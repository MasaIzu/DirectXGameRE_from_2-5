#pragma once
#include "Model.h"
#include "math/Vector3.h"
#include <Model.h>
#include "ViewProjection.h"
#include <WorldTransform.h>
#include <cassert>
#include "affin.h"
#include "Input.h"



class PlayerBullet {
public:
	/// <summary>
	///初期化
	/// <summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle_, uint32_t textureHandle);

	/// <summary>
	/// 更新処理
	/// <summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// 描画処理
	/// <summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突時に呼び出すコールバック関数
	/// <summary>
	void OnCollision();

	bool IsDead() const { return isDead_; }

	void worldSet(WorldTransform* worldTransform);

	Vector3 GetWorldPosition();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t texture2Handle_ = 0u;
	//テクスチャハンドル
	uint32_t texture4Handle_ = 0u;

	Input* input_ = nullptr;

	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

	int isHit = 0;

};