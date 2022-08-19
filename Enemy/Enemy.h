#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "DebugText.h"
#include "affin.h"
#include "EnemyBullet.h"

//���@�N���X�̑O���錾
class Player;

class Enemy {

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Move();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	void Attack();

	/// <summary>
	/// �ڋ�
	/// </summary>
	void ApproachInitialize();
	void Approach();

	/// <summary>
	/// ���E
	/// </summary>
	void Leave();

	/// <summary>
	/// �Փˎ��ɌĂяo���R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

	enum class Phase {
		Approach,//�ڋ߂���
		Leave,//���E����
	};

	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	void Fire();
	void BulletClean();

	//���ˊԊu
	static const int kFireInterval = 60;

	void  SetPlayer(Player* player) { player_ = player; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//���L����
	Player* player_ = nullptr;

	//�t���O
	int back = 0;
	//�L�����N�^�[�̈ړ��̑���
	const float kEnemyCharacterSpeed = 0.0f;
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { kEnemyCharacterSpeed, 0, 0 };

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	//�L�����N�^�[�̃t�F�[�Y�ړ��̑���
	const float kEnemyPhaseCharacterSpeed = 0.0f;

	//�e
	std::list<std::unique_ptr<EnemyBullet>>bullets_;

	int BulletTimer = 0;
};