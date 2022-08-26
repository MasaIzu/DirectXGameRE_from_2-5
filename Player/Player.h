#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "DebugText.h"
#include "affin.h"
#include "PlayerBullet.h"
#include "memory"
#include <list>


class Player {

public:
	/// <summary>
	/// ������
	/// <summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// <summary>
	void Move();

	/// <summary>
	/// �X�V
	/// <summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// �`��
	/// <summary>
	void Draw(ViewProjection viewProjection_);

	/// <summary>
	/// �U��
	/// <summary>
	void Attack();

	/// <summary>
	/// �Փˎ��ɌĂяo���R�[���o�b�N�֐�
	/// <summary>
	void OnCollision();

	/// <summary>
	/// UI�`��
	/// <summary>
	void DrawUI();

	/// <summary>
	/// Reticle�X�V
	/// <summary>
	void ReticleUpdate(const ViewProjection& viewProjection);

	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	void worldSet(WorldTransform* worldTransform);

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>>bullets_;

	//3D���e�B�N���p���[���h�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;

	//2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> sprite2DReticle_;



};