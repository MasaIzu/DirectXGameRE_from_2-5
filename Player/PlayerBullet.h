#pragma once
#include "Model.h"
#include "math/Vector3.h"
#include <Model.h>
#include "ViewProjection.h"
#include <WorldTransform.h>
#include <cassert>
#include "affin.h"
#include "Input.h"



class PlayerBullet {
public:
	/// <summary>
	///������
	/// <summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle_, uint32_t textureHandle);

	/// <summary>
	/// �X�V����
	/// <summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// �`�揈��
	/// <summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// �Փˎ��ɌĂяo���R�[���o�b�N�֐�
	/// <summary>
	void OnCollision();

	bool IsDead() const { return isDead_; }

	void worldSet(WorldTransform* worldTransform);

	Vector3 GetWorldPosition();
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t texture2Handle_ = 0u;
	//�e�N�X�`���n���h��
	uint32_t texture4Handle_ = 0u;

	Input* input_ = nullptr;

	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;

	int isHit = 0;

};