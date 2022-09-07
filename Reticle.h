#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "DebugText.h"
#include "affin.h"

class Reticle {
public:

	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// <summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// <summary>
	void Draw();

	/// <summary>
	/// Reticle更新
	/// <summary>
	void ReticleUpdate(const ViewProjection& viewProjection);

	/// <summary>
	/// マウス位置初期化
	/// <summary>
	void MousePosInitialize();

	void worldTransformSet(WorldTransform* worldTransform);
	Vector3 GetAngle();
	Vector3 GetWorldPosition();
	WorldTransform* GetWorldTransform();

private:
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	

};