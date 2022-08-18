#pragma once
#include "math/Vector3.h"
#include <Model.h>
#include "ViewProjection.h"
#include <Model.h>
#include <WorldTransform.h>
#include <cassert>
#include <affin.h>

class PlayerBullet {
public:
	///< summary>
	///初期化
	///</summary>
	void Initialize(Model* model, const Vector3& position);

	///< summary>
	///初期化
	///</summary>
	void Update();

	///< summary>
	///初期化
	///</summary>
	void Draw(const ViewProjection& viewProjection);


private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};