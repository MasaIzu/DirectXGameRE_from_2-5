#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin.h"
#include "Model.h"
#include <assert.h>


class Select {
public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* GroundModel, Model* modelDome);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldGroundTransform_;
	WorldTransform worldDomeTransform_;
	//���f��
	Model* modelDome_ = nullptr;
	Model* GroundModel_ = nullptr;
	Model* taitolModel_ = nullptr;

	//�I�����
	WorldTransform startTrans;
	//�I�����
	WorldTransform startTrans2;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;
	//�e�N�X�`���n���h��
	uint32_t textureHandle2_ = 0;

};