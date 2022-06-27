#include"Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�e�N�X�`���n���h���̏�����
	textureHandle_ = textureHandle;
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

	worldTransform_.translation_ += move;

	//�s��X�V
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	
	
}

void Player::Update(){

	Move();

}

void Player::Draw(ViewProjection& viewProjection_){

	model_->Draw(worldTransform_, viewProjection_, textureHandle_);


	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"translation : %f,%f,%f", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

