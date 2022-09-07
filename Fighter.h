#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "affin.h"


class Fighter {
public:

	/// <summary>
	/// ������
	/// <summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// <summary>
	void Update();

	/// <summary>
	/// �X�V
	/// <summary>
	void Move();

	/// <summary>
	/// �`��
	/// <summary>
	void Draw();


	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�O���x�N�g���ۑ�
	Vector3 forwardVec;


};