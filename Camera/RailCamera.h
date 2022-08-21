#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin.h"
#include "Input.h"
#include "DebugText.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3 pos,const Vector3 rad);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	ViewProjection GetViewProjection();
	WorldTransform* GetworldTransform();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//キーインプット
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

};