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

	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);

	ViewProjection GetViewProjection();
	WorldTransform* GetWorldTransform();
	Vector3 GetTarget();
	//カメラを動かすなら0止めるなら1
	void cameraSet(int x);

	void setShake(int x);
	//void SetMouseMovemont(Vector3 mouseMovement);
	void MousePosInitialize();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//キーインプット
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	Vector2 MouseMove;
	//マウスの移動量保存
	Vector2 mouseMoved;
	Vector3 Target;

	Vector3 cameraMoveY{ 0,0,0 };

	int isShake = 0;
	int cameraMoveFlag = 0;
};