#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "affin.h"


class Fighter {
public:

	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// <summary>
	void Update();

	/// <summary>
	/// 更新
	/// <summary>
	void Move();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw();


	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);

private:

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//前方ベクトル保存
	Vector3 forwardVec;


};