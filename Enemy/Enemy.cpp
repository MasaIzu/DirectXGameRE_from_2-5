#include"Enemy.h"
#include"Player/Player.h"
#include <cmath>
#include <GameScene.h>
using namespace MathUtility;

void Enemy::Initialize(Model* model, uint32_t textureHandle, uint32_t textureHandle_50, Vector3 vector3, Model* HpModel) {
	//NULL�|�C���^�`�F�b�N
	model_ = model;
	HpModel_ = HpModel;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransformEnemyHp_.Initialize();

	worldTransform_.translation_ = vector3;
	worldTransformEnemyHp_.translation_ = vector3 + Vector3(0, 5, 0);
	MovePhase = worldTransform_.translation_;

	KeepY = worldTransform_.translation_.y;

	enemyTextureHandle_ = textureHandle;
	enemy50TextureHandle_ = textureHandle_50;

	worldTransformEnemyHp_.scale_ = { 6,1,1 };
	enemyHP = 2;
	//�s��X�V
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();

	AffinTrans::affin(worldTransformEnemyHp_);
	worldTransformEnemyHp_.TransferMatrix();

	audio = Audio::GetInstance();
	shotHandol = audio->LoadWave("bomb.mp3");

	ApproachInitialize();
}

void Enemy::Move() {

	//�͈͂𒴂��Ȃ�����
	if (worldTransform_.translation_.x <= -kMoveLimitX) {
		move = { kEnemyCharacterSpeed, 0, 0 };
	}
	if (worldTransform_.translation_.x >= kMoveLimitX) {
		move = { -kEnemyCharacterSpeed, 0, 0 };
	}
	worldTransform_.rotation_ += rot;
	worldTransform_.translation_ += move;
	worldTransformEnemyHp_.translation_ += move;
	//�s��X�V
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();

	AffinTrans::affin(worldTransformEnemyHp_);
	worldTransformEnemyHp_.TransferMatrix();

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	case Phase::Up:
		Up();
		break;
	case Phase::Near:
		Near();
		break;
	case Phase::Back:
		Back();
		break;
	}
}

void Enemy::Update() {

	Move();

	//���˃^�C�}�[�J�E���g�_�E��
	BulletTimer--;
	//���ԂɒB������
	if (BulletTimer <= 0) {
		if (enemyAttackFlag == 0) {
			Attack();			//�e�𔭎�
			BulletTimer = 90;	//���˃^�C�}�[��������
		}
	}

}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
	if (enemyHP == 2) {
		HpModel_->Draw(worldTransformEnemyHp_, viewProjection_, enemyTextureHandle_);
	}
	else {
		HpModel_->Draw(worldTransformEnemyHp_, viewProjection_, enemy50TextureHandle_);
	}
}

void Enemy::ApproachInitialize() {

	//���˃^�C�}�[�̏�����
	BulletTimer = kFireInterval;

}

void Enemy::Approach() {

	//�ړ��x�N�g��
	worldTransform_.translation_ += Vector3(0, 0, -kEnemyPhaseCharacterSpeed);
	worldTransformEnemyHp_.translation_ += Vector3(0, 0, -kEnemyPhaseCharacterSpeed);
	//����̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < -120.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {

	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += Vector3(0, 0, +kEnemyPhaseCharacterSpeed);
	worldTransformEnemyHp_.translation_ += Vector3(0, 0, +kEnemyPhaseCharacterSpeed);
	//����̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z > 40.0f) {
		phase_ = Phase::Up;
	}
}

void Enemy::Up() {

	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += Vector3(0, 0.1f, +kEnemyPhaseCharacterSpeed);
	worldTransformEnemyHp_.translation_ += Vector3(0, 0.1f, +kEnemyPhaseCharacterSpeed);

	//����̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.y > KeepY + 100.0f) {
		MoveTimer = 1200;
		phase_ = Phase::Near;
	}

}

void Enemy::Near() {

	MoveTimer--;
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += MovePhase * 0.2f;
	worldTransformEnemyHp_.translation_ += MovePhase * 0.2f;
	if (MoveTimer < 0) {
		MoveTimer = 600;
		phase_ = Phase::Back;
	}

}

void Enemy::Back() {
	MoveTimer--;
	worldTransform_.translation_ += AffinTrans::vector3Normalize(AffinTrans::SubVec(MovePhase, worldTransform_.translation_)) * 0.2f;
	worldTransformEnemyHp_.translation_ += AffinTrans::vector3Normalize(AffinTrans::SubVec(MovePhase, worldTransform_.translation_)) * 0.2f;
	if (MoveTimer < 0) {
		phase_ = Phase::Approach;
	}
}

void Enemy::OnCollision() {
	enemyHP--;
	if (enemyHP < 0) {
		isDead_ = true;
		audio->PlayWave(shotHandol, false, 0.4f);
	}
}

void Enemy::Attack() {

	//�e�̑��x
	const float kBulletSpeed = 1.2f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = bVelocity(velocity, worldTransform_);

	Vector3 PlayerVec = player_->GetWorldPosition();
	PlayerVec -= Vector3(3, 0, 0);
	Vector3 EnemyVec = worldTransform_.translation_;

	Vector3 A_BVec = Vector3(PlayerVec.x - EnemyVec.x, PlayerVec.y - EnemyVec.y, PlayerVec.z - EnemyVec.z);
	float nomalize = sqrt(A_BVec.x * A_BVec.x + A_BVec.y * A_BVec.y + A_BVec.z * A_BVec.z);
	A_BVec = Vector3(A_BVec.x / nomalize, A_BVec.y / nomalize, A_BVec.z / nomalize);
	MovePhase = A_BVec;
	MovePhase = bVelocity(MovePhase, worldTransform_);
	velocity = A_BVec * kBulletSpeed;

	// �e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	gameScene_->AddEnemyBullet(newBullet);
	////���X�g�ɓo�^����
	//enemyBullets_.push_back(std::move(enemyBullet));

}

Vector3 Enemy::bVelocity(Vector3& velocity, WorldTransform& worldTransform) {

	Vector3 result = { 0, 0, 0 };

	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];


	return result;
}


Vector3 Enemy::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::attackSet(int x) {
	enemyAttackFlag = x;
}