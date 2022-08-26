#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "DebugText.h"
#include "affin.h"
#include "PlayerBullet.h"
#include "memory"
#include <list>


class Player {

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
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// <summary>
	void Draw(ViewProjection viewProjection_);

	/// <summary>
	/// 攻撃
	/// <summary>
	void Attack();

	/// <summary>
	/// 衝突時に呼び出すコールバック関数
	/// <summary>
	void OnCollision();

	/// <summary>
	/// UI描画
	/// <summary>
	void DrawUI();

	/// <summary>
	/// Reticle更新
	/// <summary>
	void ReticleUpdate(const ViewProjection& viewProjection);

	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	void worldSet(WorldTransform* worldTransform);

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>>bullets_;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite> sprite2DReticle_;



};