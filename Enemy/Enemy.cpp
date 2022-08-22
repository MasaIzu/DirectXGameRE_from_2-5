#include"Enemy.h"
#include"Player/Player.h"
#include <cmath>
#include <GameScene.h>

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 vector3) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = vector3;

	//�s��X�V
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();

	ApproachInitialize();

}

void Enemy::Move() {

	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;


	//�͈͂𒴂��Ȃ�����
	if (worldTransform_.translation_.x <= -kMoveLimitX) {
		move = { kEnemyCharacterSpeed, 0, 0 };
	}
	if (worldTransform_.translation_.x >= kMoveLimitX) {
		move = { -kEnemyCharacterSpeed, 0, 0 };
	}

	worldTransform_.translation_ += move;
	//�s��X�V
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}


	debugText_->SetPos(400, 20);
	debugText_->Printf(
		"PlayerVec : %f,%f,%f", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Enemy::Update() {

	
	Move();

}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

}

void Enemy::ApproachInitialize() {

	//���˃^�C�}�[�̏�����
	BulletTimer = kFireInterval;

}

void Enemy::Approach() {
	//���˃^�C�}�[�J�E���g�_�E��
	BulletTimer--;
	//���ԂɒB������
	if (BulletTimer <= 0) {
		Attack();				//�e�𔭎�
		BulletTimer = 60;	//���˃^�C�}�[��������
	}

	//�ړ��x�N�g��
	worldTransform_.translation_ += Vector3(0, 0, -kEnemyPhaseCharacterSpeed);
	//����̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < -20.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	//���˃^�C�}�[�J�E���g�_�E��
	BulletTimer--;
	//���ԂɒB������
	if (BulletTimer <= 0) {
		Attack();				//�e�𔭎�
		BulletTimer = 60;	//���˃^�C�}�[��������
	}
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += Vector3(0, 0, +kEnemyPhaseCharacterSpeed);
	//����̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z > 20.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::OnCollision(){
	isDead_ = true;
}

void Enemy::Attack() {

	//�e�̑��x
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = bVelocity(velocity, worldTransform_);

	Vector3 PlayerVec = player_->GetWorldPosition();
	Vector3 EnemyVec = GetWorldPosition();

	Vector3 A_BVec = Vector3(PlayerVec.x - EnemyVec.x, PlayerVec.y - EnemyVec.y, PlayerVec.z - EnemyVec.z);
	float nomalize = sqrt(A_BVec.x * A_BVec.x + A_BVec.y * A_BVec.y + A_BVec.z * A_BVec.z) * 10;
	A_BVec = Vector3(A_BVec.x / nomalize, A_BVec.y / nomalize, A_BVec.z / nomalize);


	// �e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, A_BVec);

	//�e��o�^����
	gameScene_->AddEnemyBullet(newBullet);
	////���X�g�ɓo�^����
	//enemyBullets_.push_back(std::move(enemyBullet));

	debugText_->SetPos(400, 20);
	debugText_->Printf(
		"PlayerVec : %f,%f,%f", PlayerVec.x,
		PlayerVec.y,
		PlayerVec.z);
	
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


Vector3 Enemy::GetWorldPosition(){
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
