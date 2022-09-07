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
	/// ������
	/// <summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// <summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// �`��
	/// <summary>
	void Draw();

	/// <summary>
	/// Reticle�X�V
	/// <summary>
	void ReticleUpdate(const ViewProjection& viewProjection);

	/// <summary>
	/// �}�E�X�ʒu������
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