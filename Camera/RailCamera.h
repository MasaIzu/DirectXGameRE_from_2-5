#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin.h"
#include "Input.h"
#include "DebugText.h"

/// <summary>
/// ���[���J����
/// </summary>
class RailCamera {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3 pos,const Vector3 rad);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	ViewProjection GetViewProjection();
	WorldTransform* GetworldTransform();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	//�L�[�C���v�b�g
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

};