#include "RailCamera.h"

void RailCamera::Initialize(const Vector3 pos, const Vector3 rad){
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rad;
	worldTransform_.Initialize();

	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.Initialize();
	input_ = Input::GetInstance();

}

void RailCamera::Update(){
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 rotat = { 0, 0, 0 };
	//�L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.1f;

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 cameraMove = { 0, 0, 0 };
	//�L�����N�^�[�̈ړ��̑���
	const float cameraSpeed = 0.005f;


	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_W)) {
		cameraMove = { 0, kCharacterSpeed, 0 };
	}
	else if (input_->PushKey(DIK_S)) {
		cameraMove = { 0, -kCharacterSpeed, 0 };
	}
	if (input_->PushKey(DIK_A)) {
		cameraMove = { -kCharacterSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_D)) {
		cameraMove = { kCharacterSpeed, 0, 0 };
	}
	if (input_->PushKey(DIK_E)) {
		cameraMove = { 0, 0, -kCharacterSpeed };
	}
	else if (input_->PushKey(DIK_R)) {
		cameraMove = { 0, 0, kCharacterSpeed };
	}

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_H)) {
		rotat = { 0, cameraSpeed, 0 };
	}
	else if (input_->PushKey(DIK_F)) {
		rotat = { 0, -cameraSpeed, 0 };
	}
	if (input_->PushKey(DIK_T)) {
		rotat = { -cameraSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_G)) {
		rotat = { cameraSpeed, 0, 0 };
	}
	if (input_->PushKey(DIK_U)) {
		rotat = { 0, 0, -cameraSpeed };
	}
	else if (input_->PushKey(DIK_Y)) {
		rotat = { 0, 0, cameraSpeed };
	}

	worldTransform_.rotation_ += rotat;
	worldTransform_.translation_ += cameraMove;

	//�s��X�V
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();

	viewProjection_.eye = worldTransform_.translation_;
	
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = AffinTrans::MatVector(worldTransform_.matWorld_, forward);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;
	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	viewProjection_.up = AffinTrans::MatVector(worldTransform_.matWorld_,up);
	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();


}

ViewProjection RailCamera::GetViewProjection(){
	return viewProjection_;
}

WorldTransform* RailCamera::GetworldTransform(){
	return &worldTransform_;
}

