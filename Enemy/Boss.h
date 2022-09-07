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

//GameScene�̑O���錾(����̍�)
class GameScene;

class Boss {

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model,Vector3 vector3, Model* handmodel, Model* minModel);

	/// <summary>
	/// �X�V
	/// </summary>
	void Move();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const Vector3 worldTransform);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	void Attack(const Vector3 worldTransform);

	/// <summary>
	/// �ڋ�
	/// </summary>
	void Approach();

	/// <summary>
	/// ���E
	/// </summary>
	void Leave();

	/// <summary>
	/// Up
	/// </summary>
	void Up();

	/// <summary>
	/// Near
	/// </summary>
	void Near();

	/// <summary>
	/// �߂�
	/// </summary>
	void Back();

	/// <summary>
	/// �Փˎ��ɌĂяo���R�[���o�b�N�֐�
	/// </summary>
	void OnCollisionBoss();
	void OnCollisionRight();
	void OnCollisionLeft();



	bool IsDead() const { return isDead_; }

	enum class Phase {
		Approach,//�ڋ߂���
		Leave,//���E����
		Up,//�㏸
		Near,//�߂Â�
		Back,//�����
	};

	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);


	//���ˊԊu
	static const int kFireInterval = 60;

	void  SetPlayer(Player* player) { player_ = player; }

	//���[���h���W���擾
	Vector3 GetWorldPositionBoss();
	Vector3 GetWorldPositionRight();
	Vector3 GetWorldPositionLeft();

	//�Q�[���V�[��Setter
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//�łȂ�0�ł��Ȃ��Ȃ�1
	void attackSet(int x);

	int GetBossHp();

private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransformBoss_;
	WorldTransform worldTransformBossRightHand_;
	WorldTransform worldTransformBossLeftHand_;
	//���f��
	Model* model_ = nullptr;
	Model* minModel_ = nullptr;
	Model* handModel_ = nullptr;
	DebugText* debugText_ = nullptr;
	//�Q�[���V�[��
	GameScene* gameScene_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t enemyTextureHandle_ = 0;
	//�e�N�X�`���n���h��
	uint32_t enemy50TextureHandle_ = 0;

	//���L����
	Player* player_ = nullptr;

	//enemyHp
	int bossHP = 0;

	//�t���O
	int back = 0;
	//�L�����N�^�[�̈ړ��̑���
	const float kEnemyCharacterSpeed = 0.2f;
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { kEnemyCharacterSpeed, 0, 0 };
	Vector3 rot = { 0,0.01f,0 };
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	//�L�����N�^�[�̃t�F�[�Y�ړ��̑���
	const float kEnemyPhaseCharacterSpeed = 0.1f;
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;
	//�f�X�t���O
	bool isDead_ = false;

	int BulletTimer = 0;

	int phaseTimer = 0;

	float KeepY = 0;
	Vector3 keep;
	Vector3 MovePhase{ 0,0,0 };
	Vector3 BackPhase{ 0,0,0 };
	int MoveTimer;

	int enemyAttackFlag = 0;

	int upDown = 0;
	int a = 0;
};