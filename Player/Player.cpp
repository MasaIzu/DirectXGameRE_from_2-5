#include "Player.h"


void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	int i = 0; int ibuf;
	for (WorldTransform& worldTransform : worldTransforms_) {

		//���[���h�g�����X�t�H�[���̏�����
		worldTransform.Initialize();

		
		i++;
		if (i < 26) {
			// X,Y,Z���̎���̕��s�ړ���ݒ�
			worldTransform.translation_ = Vector3(0 + 2 * i, 0, 0);
		}
		else {
			ibuf = i;
			i = i - 25;
			// X,Y,Z���̎���̕��s�ړ���ݒ�
			worldTransform.translation_ = Vector3(0 + 2 * i, 5, 0);
			i = ibuf;
		}

		//�����̌v�Z
		worldTransform.matWorld_.m[0][0] = 1.0f;
		worldTransform.matWorld_.m[1][1] = 1.0f;
		worldTransform.matWorld_.m[2][2] = 1.0f;
		worldTransform.matWorld_.m[3][3] = 1.0f;

#pragma region �X�P�[�����O

		//Matrix4 mat = MathUtility::Matrix4Identity();

		worldTransform.matWorld_ = AffinTrans::Initialize();
		worldTransform.matWorld_ *= AffinTrans::Scale(Vector3(worldTransform.scale_.x, worldTransform.scale_.y, worldTransform.scale_.z));
	}
}

void Player::Move() {

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0, 0, 0 };
	//�L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.2f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_UP)) {
		move = { 0, kCharacterSpeed, 0 };
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { 0, -kCharacterSpeed, 0 };
	}
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharacterSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharacterSpeed, 0, 0 };
	}

	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.translation_ += move;

		//�s��X�V
		AffinTrans::affin(worldTransform);

		worldTransform.TransferMatrix();

		const float kMoveLimitX = 35;
		const float kMoveLimitY = 18;


		const float kChestRotSpeed = 0.05f;

		//�����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_U)) {
			worldTransform.rotation_.y -= kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_I)) {
			worldTransform.rotation_.y += kChestRotSpeed;
		}
	}
	//�e���ˏ���
	Attack();

	//�e�X�V
	if (bullet_) {
		bullet_->Update();
	}

}

void Player::Update() {

	Move();

}

void Player::Draw(ViewProjection& viewProjection_) {
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}
	//�e�X�V
	if (bullet_) {
		bullet_->Draw(viewProjection_);
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
		//�e�𐶐���������
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransforms_[0].translation_);

		//�e�̓o�^
		bullet_ = newBullet;
	}

}