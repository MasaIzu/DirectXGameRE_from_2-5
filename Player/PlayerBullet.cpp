#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity, uint32_t textureHandle_, uint32_t textureHandle) {
	assert(model);

	model_ = model;

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	worldTransform_.translation_ += Vector3(0, -2, 0);
	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	texture2Handle_ = textureHandle_;
	texture4Handle_ = textureHandle;

	isHit = 0;
	//�s��X�V
	AffinTrans::affin(worldTransform_);
	//worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Update(const ViewProjection& viewProjection) {

	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;


	//worldTransform_.rotation_ += Vector3(0.1f, 0.1f, 0.1f);

	//�s��X�V
	AffinTrans::affin(worldTransform_);
	worldTransform_.matWorld_ *= viewProjection.matView;
	worldTransform_.TransferMatrix();

	//���ԂŃf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {


	model_->Draw(worldTransform_, viewProjection);

	if (isHit == 1) {

	}
}

void PlayerBullet::OnCollision() {
	isHit = 1;
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition() {

	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::worldSet(WorldTransform* worldTransform) {

	worldTransform_.parent_ = worldTransform->parent_;

}

