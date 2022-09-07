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
#include"audio/Audio.h"


class Player {

public:
	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize(Model* model);

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
	void Attack(const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突時に呼び出すコールバック関数
	/// <summary>
	void OnCollision();

	/// <summary>
	/// Reticle更新
	/// <summary>
	void ReticleUpdate(const ViewProjection& viewProjection);

	/// <summary>
	/// Reticle描画
	/// <summary>
	void DrawSprite();

	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	int GetShake();
	int GetPlayerHp();

	void worldReticleSet(WorldTransform* worldTransform);
	void SetTarget(Vector3 target);

	void setDrawSelect(int x);
	//void worldRailCameraSet(WorldTransform* worldTransform);

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//レティクル用テクスチャ取得
	uint32_t textureReticle;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	//モデル
	Model* playerModel_ = nullptr;
	//バレットモデル
	Model* modelBullet_ = nullptr;

	//レティクル用テクスチャ取得
	uint32_t texture2dame;
	uint32_t texture4dame;

	//弾
	std::list<std::unique_ptr<PlayerBullet>>bullets_;

	int BulletTimer = 0;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite> sprite2DReticle_;

	//マウスの移動量保存
	Vector3 reticlePos;
	Vector3 target_;

	int playerHp = 7;

	float BulletDistance;

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite> spriteHP_FULL;
	std::unique_ptr<Sprite> spriteHP_1;
	std::unique_ptr<Sprite> spriteHP_2;
	std::unique_ptr<Sprite> spriteHP_3;
	std::unique_ptr<Sprite> spriteHP_4;
	std::unique_ptr<Sprite> spriteHP_5;
	std::unique_ptr<Sprite> spriteHP_6;
	std::unique_ptr<Sprite> spriteHP_7;
	//HP用テクスチャ取得
	uint32_t textureReticle_FULL;
	uint32_t textureReticle_1;
	uint32_t textureReticle_2;
	uint32_t textureReticle_3;
	uint32_t textureReticle_4;
	uint32_t textureReticle_5;
	uint32_t textureReticle_6;
	uint32_t textureReticle_7;

	Audio* audio = nullptr;
	uint32_t shotHandol = 0;
	uint32_t damegeHandol = 0;

	int DrawHpSelect = 0;

	int isShake = 0;
	int shakeTime = 10;

	int isHit = 0;

};