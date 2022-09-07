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
#include"audio/Audio.h"


class Player {

public:
	/// <summary>
	/// ������
	/// <summary>
	void Initialize(Model* model);

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
	void Attack(const ViewProjection& viewProjection);

	/// <summary>
	/// �Փˎ��ɌĂяo���R�[���o�b�N�֐�
	/// <summary>
	void OnCollision();

	/// <summary>
	/// Reticle�X�V
	/// <summary>
	void ReticleUpdate(const ViewProjection& viewProjection);

	/// <summary>
	/// Reticle�`��
	/// <summary>
	void DrawSprite();

	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	int GetShake();
	int GetPlayerHp();

	void worldReticleSet(WorldTransform* worldTransform);
	void SetTarget(Vector3 target);

	void setDrawSelect(int x);
	//void worldRailCameraSet(WorldTransform* worldTransform);

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//���e�B�N���p�e�N�X�`���擾
	uint32_t textureReticle;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	//���f��
	Model* playerModel_ = nullptr;
	//�o���b�g���f��
	Model* modelBullet_ = nullptr;

	//���e�B�N���p�e�N�X�`���擾
	uint32_t texture2dame;
	uint32_t texture4dame;

	//�e
	std::list<std::unique_ptr<PlayerBullet>>bullets_;

	int BulletTimer = 0;

	//3D���e�B�N���p���[���h�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;

	//2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> sprite2DReticle_;

	//�}�E�X�̈ړ��ʕۑ�
	Vector3 reticlePos;
	Vector3 target_;

	int playerHp = 7;

	float BulletDistance;

	//2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> spriteHP_FULL;
	std::unique_ptr<Sprite> spriteHP_1;
	std::unique_ptr<Sprite> spriteHP_2;
	std::unique_ptr<Sprite> spriteHP_3;
	std::unique_ptr<Sprite> spriteHP_4;
	std::unique_ptr<Sprite> spriteHP_5;
	std::unique_ptr<Sprite> spriteHP_6;
	std::unique_ptr<Sprite> spriteHP_7;
	//HP�p�e�N�X�`���擾
	uint32_t textureReticle_FULL;
	uint32_t textureReticle_1;
	uint32_t textureReticle_2;
	uint32_t textureReticle_3;
	uint32_t textureReticle_4;
	uint32_t textureReticle_5;
	uint32_t textureReticle_6;
	uint32_t textureReticle_7;

	Audio* audio = nullptr;
	uint32_t shotHandol = 0;
	uint32_t damegeHandol = 0;

	int DrawHpSelect = 0;

	int isShake = 0;
	int shakeTime = 10;

	int isHit = 0;

};