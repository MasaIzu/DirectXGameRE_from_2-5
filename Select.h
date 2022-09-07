#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin.h"
#include "Model.h"
#include <assert.h>


class Select {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* GroundModel, Model* modelDome);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private:

	//ワールド変換データ
	WorldTransform worldGroundTransform_;
	WorldTransform worldDomeTransform_;
	//モデル
	Model* modelDome_ = nullptr;
	Model* GroundModel_ = nullptr;
	Model* taitolModel_ = nullptr;

	//選択画面
	WorldTransform startTrans;
	//選択画面
	WorldTransform startTrans2;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//テクスチャハンドル
	uint32_t textureHandle2_ = 0;

};