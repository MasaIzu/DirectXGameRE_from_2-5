#include "Player.h"
#include "WinApp.h"
#include "MathUtility.h"
using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = { -7,-5,20 };

	//3D���e�B�N���̃��[���h�g�����X�t�H�[��������
	worldTransform3DReticle_.Initialize();

	//���e�B�N���p�e�N�X�`���擾
	uint32_t textureReticle = TextureManager::Load("aim.png");
	//�X�v���C�g����
	sprite2DReticle_.reset(Sprite::Create(textureReticle, Vector2(360, 250), Vector4(1, 1, 1, 1),Vector2(0.5, 0.5)));
	
	

}

void Player::Move() {

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0, 0, 0 };
	//�L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.1f;

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

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

	//�Q�[���p�b�h�̏�Ԃ�m��ϐ�(XINPUT)
	XINPUT_STATE joyState;

	//�W���C�X�e�B�b�N���
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}


	worldTransform_.translation_ += move;

	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	const float kChestRotSpeed = 0.05f;

	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= kChestRotSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += kChestRotSpeed;
	}

	//�s��X�V
	AffinTrans::affin(worldTransform_);

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	worldTransform_.TransferMatrix();

	debugText_->SetPos(100, 20);
	debugText_->Printf(
		"PlayerVec : %f,%f,%f", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);


}

void Player::ReticleUpdate(const ViewProjection& viewProjection){

#pragma region //���@�̃��[���h���W����3D���e�B�N���̃��[���h���W���v�Z

	//���@����3D���e�B�N���ւ̋���
	const float kDistancePlayerTo3DReticle = 30.0f;
	//���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z+����)
	Vector3 offset = { 0,0,1.0f };
	//���@�̃��[���h���W�̉�]�𔽉f
	offset = AffinTrans::MatVector(worldTransform_.matWorld_, offset);
	//�x�N�g���̒����𐮂���
	offset = Vector3Normalize(offset) * kDistancePlayerTo3DReticle;
	//3D���e�B�N���̍��W��ݒ�
	worldTransform3DReticle_.translation_ = AffinTrans::AddVector3(offset, AffinTrans::GetWorldTransform(worldTransform_.matWorld_));

	//�s��X�V
	AffinTrans::affin(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();

#pragma endregion

#pragma region //3D���e�B�N���̃��[���h���W����2D���e�B�N���̃X�N���[�����W���v�Z

	Vector3 positionReticle = AffinTrans::GetWorldTransform(worldTransform3DReticle_.matWorld_);

	Vector2 windowWH = Vector2(WinApp::GetInstance()->kWindowWidth, WinApp::GetInstance()->kWindowHeight);

	//�r���[�|�[�g�s��
	Matrix4 Viewport =
	{ windowWH.x / 2,0,0,0,
	0,-windowWH.y / 2,0,0,
	0,0,1,0,
	windowWH.x / 2, windowWH.y / 2,0,1 };

	//�r���[�s��ƃv���W�F�N�V�����s��A�r���[�|�[�g�s�����������
	Matrix4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * Viewport;

	//���[���h���X�N���[�����W�ϊ�(������3D����2D�ɂȂ�)
	positionReticle = AffinTrans::DivVecMat(positionReticle, matViewProjectionViewport);

	//�X�v���C�g�̃��e�B�N���ɍ��W�ݒ�
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

#pragma endregion

#pragma region //�X�N���[�����W����2D���e�B�N���̃��[���h���W���v�Z
	POINT mousePosition;
	//�}�E�X���W(�X�N���[�����W)���擾����
	GetCursorPos(&mousePosition);

	//�N���C�A���g�G���A���W�ɕϊ�����
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));

	//�r���[�|�[�g�s�� (������ɂ���)
	/*Matrix4 Viewport =
	{ windowWH.x / 2,0,0,0,
	0,-windowWH.y / 2,0,0,
	0,0,1,0,
	windowWH.x / 2, windowWH.y / 2,0,1 };*/

	//�r���[�v���W�F�N�V�����r���[�|�[�g����
	Matrix4 matVPV = viewProjection.matView * viewProjection.matProjection * Viewport;

	//�����s��̋t�s����v�Z����
	Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);
	//�X�N���[�����W
	Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
	Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

	//�X�N���[�����W�n���烏�[���h���W�n��
	posNear = AffinTrans::DivVecMat(posNear, matInverseVPV);
	posFar = AffinTrans::DivVecMat(posFar, matInverseVPV);

	//�}�E�X���C�̕���
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Vector3Normalize(mouseDirection);
	//�J��������Ə��I�u�W�F�N�g�̋���
	const float kDistanceTestObject = 80.0f;
	worldTransform3DReticle_.translation_ = AffinTrans::AddVector3(posNear, mouseDirection * kDistanceTestObject);

	//�s��X�V
	AffinTrans::affin(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();

#pragma endregion

#pragma region //�W���C�X�e�B�b�N�ł̃��e�B�N��

	//�X�v���C�g�̌��ݍ��W���擾
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyState;

	//�W���C�X�e�B�b�N��Ԏ擾
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		//�X�v���C�g�̍��W�ύX�𔽉f
		sprite2DReticle_->SetPosition(spritePosition);
	}

#pragma endregion

}

void Player::Update(const ViewProjection& viewProjection) {

	Move();
	ReticleUpdate(viewProjection);
	//�e���ˏ���
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

}

void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::DrawUI(){

	sprite2DReticle_->Draw();

}


void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = bVelocity(velocity, worldTransform_);

		//���@����Ə��I�u�W�F�N�g�ւ̃x�N�g��
		velocity = AffinTrans::GetWorldTransform(worldTransform3DReticle_.matWorld_) - AffinTrans::GetWorldTransform(worldTransform_.matWorld_);
		velocity = Vector3Normalize(velocity) * kBulletSpeed;

		//�e�𐶐���������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, AffinTrans::GetWorldTransform(worldTransform_.matWorld_), velocity);

		//�e�̓o�^
		bullets_.push_back(std::move(newBullet));
	}

	XINPUT_STATE joyState;
	//�Q�[���p�b�h���ڑ��Ȃ�Ή�������������
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	//R�g���K�[�������Ă�����
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = bVelocity(velocity, worldTransform_);

		//���@����Ə��I�u�W�F�N�g�ւ̃x�N�g��
		velocity = AffinTrans::GetWorldTransform(worldTransform3DReticle_.matWorld_) - AffinTrans::GetWorldTransform(worldTransform_.matWorld_);
		velocity = Vector3Normalize(velocity) * kBulletSpeed;

		//�e�𐶐���������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, AffinTrans::GetWorldTransform(worldTransform_.matWorld_), velocity);

		//�e�̓o�^
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::OnCollision() {

}

Vector3 Player::bVelocity(Vector3 velocity, WorldTransform& worldTransform) {

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

Vector3 Player::GetWorldPosition() {

	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}



void Player::worldSet(WorldTransform* worldTransform) {

	worldTransform_.parent_ = worldTransform;

}
