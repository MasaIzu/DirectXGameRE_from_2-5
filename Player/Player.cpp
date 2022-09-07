#include "Player.h"
#include "WinApp.h"
#include "MathUtility.h"
using namespace MathUtility;

void Player::Initialize(Model* model) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 2, -2, 3 };

	//3D���f���̍쐬
	modelBullet_ = Model::CreateFromOBJ("Bullet", true);
	playerModel_ = Model::CreateFromOBJ("juu", true);
	//3D���e�B�N���̃��[���h�g�����X�t�H�[��������
	worldTransform3DReticle_.Initialize();

	//���e�B�N���p�e�N�X�`���擾
	textureReticle = TextureManager::Load("aim.png");
	//�X�v���C�g����
	sprite2DReticle_.reset(Sprite::
		Create(textureReticle, Vector2(WinApp::GetInstance()->kWindowWidth / 2, WinApp::GetInstance()->kWindowHeight / 2 + 12),
			Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	textureReticle_FULL = TextureManager::Load("HPbar/HPfull.png");
	textureReticle_1 = TextureManager::Load("HPbar/HP_1.png");
	textureReticle_2 = TextureManager::Load("HPbar/HP_2.png");
	textureReticle_3 = TextureManager::Load("HPbar/HP_3.png");
	textureReticle_4 = TextureManager::Load("HPbar/HP_4.png");
	textureReticle_5 = TextureManager::Load("HPbar/HP_5.png");
	textureReticle_6 = TextureManager::Load("HPbar/HP_6.png");
	textureReticle_7 = TextureManager::Load("HPbar/HP_ALL.png");


	texture2dame = TextureManager::Load("2dame.png");
	texture4dame = TextureManager::Load("4dame.png");


	spriteHP_FULL.reset(Sprite::
		Create(textureReticle_FULL, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_1.reset(Sprite::
		Create(textureReticle_1, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_2.reset(Sprite::
		Create(textureReticle_2, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_3.reset(Sprite::
		Create(textureReticle_3, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_4.reset(Sprite::
		Create(textureReticle_4, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_5.reset(Sprite::
		Create(textureReticle_5, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_6.reset(Sprite::
		Create(textureReticle_6, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));
	spriteHP_7.reset(Sprite::
		Create(textureReticle_7, Vector2(300, 640), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	DrawHpSelect = 0;
	playerHp = 7;
	isShake = 0;
	shakeTime = 10;

	audio = Audio::GetInstance();
	shotHandol = audio->LoadWave("shot.mp3");
	damegeHandol = audio->LoadWave("dameji.mp3");

	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();
}

void Player::Move() {

	////�L�����N�^�[�̈ړ��x�N�g��
	//Vector3 move = { 0, 0, 0 };
	////�L�����N�^�[�̈ړ��̑���
	//const float kCharacterSpeed = 0.1f;

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	////�����������ňړ��x�N�g����ύX
	//if (input_->PushKey(DIK_UP)) {
	//	move = { 0, kCharacterSpeed, 0 };
	//}
	//else if (input_->PushKey(DIK_DOWN)) {
	//	move = { 0, -kCharacterSpeed, 0 };
	//}
	//if (input_->PushKey(DIK_LEFT)) {
	//	move = { -kCharacterSpeed, 0, 0 };
	//}
	//else if (input_->PushKey(DIK_RIGHT)) {
	//	move = { kCharacterSpeed, 0, 0 };
	//}

	////�Q�[���p�b�h�̏�Ԃ�m��ϐ�(XINPUT)
	//XINPUT_STATE joyState;

	////�W���C�X�e�B�b�N���
	//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
	//	move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	//}


	//worldTransform_.translation_ += move;

	//�s��X�V
	AffinTrans::affin(worldTransform_);
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	worldTransform_.TransferMatrix();
}

void Player::ReticleUpdate(const ViewProjection& viewProjection) {


#pragma region //���@�̃��[���h���W����3D���e�B�N���̃��[���h���W���v�Z

	////���@����3D���e�B�N���ւ̋���
	//const float kDistancePlayerTo3DReticle = 60.0f;
	////���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z+����)
	//Vector3 offset = { 0,0,1.0f };
	////���@�̃��[���h���W�̉�]�𔽉f
	//offset = AffinTrans::MatVector(worldTransform_.matWorld_, offset);
	////�x�N�g���̒����𐮂���
	//offset = Vector3Normalize(offset) * kDistancePlayerTo3DReticle;
	////3D���e�B�N���̍��W��ݒ�
	//worldTransform3DReticle_.translation_ = AffinTrans::AddVector3(offset, AffinTrans::GetWorldTransform(worldTransform_.matWorld_));
	//worldTransform3DReticle_.translation_ += Vector3(0, 1, 0);
	////�s��X�V
	//AffinTrans::affin(worldTransform3DReticle_);
	//worldTransform3DReticle_.TransferMatrix();

#pragma endregion

#pragma region //3D���e�B�N���̃��[���h���W����2D���e�B�N���̃X�N���[�����W���v�Z

	//Vector3 positionReticle = AffinTrans::GetWorldTransform(worldTransform3DReticle_.matWorld_);

	//Vector2 windowWH = Vector2(WinApp::GetInstance()->kWindowWidth, WinApp::GetInstance()->kWindowHeight);

	////�r���[�|�[�g�s��
	//Matrix4 Viewport =
	//{ windowWH.x / 2,0,0,0,
	//0,-windowWH.y / 2,0,0,
	//0,0,1,0,
	//windowWH.x / 2, windowWH.y / 2,0,1 };

	////�r���[�s��ƃv���W�F�N�V�����s��A�r���[�|�[�g�s�����������
	//Matrix4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * Viewport;

	////���[���h���X�N���[�����W�ϊ�(������3D����2D�ɂȂ�)
	//positionReticle = AffinTrans::DivVecMat(positionReticle, matViewProjectionViewport);

	////�X�v���C�g�̃��e�B�N���ɍ��W�ݒ�
	//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

#pragma endregion

#pragma region //�X�N���[�����W����2D���e�B�N���̃��[���h���W���v�Z

	//Vector2 windowWH = Vector2(WinApp::GetInstance()->kWindowWidth / 2, WinApp::GetInstance()->kWindowHeight / 2);

	//{
	//	//POINT mousePosition;
	//	////�}�E�X���W(�X�N���[�����W)���擾����
	//	//GetCursorPos(&mousePosition);
	//	////�N���C�A���g�G���A���W�ɕϊ�����
	//	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//	//ScreenToClient(hwnd, &mousePosition);
	//	//int xPos_absolute, yPos_absolute;
	//	//int xPos = windowWH.x;  //�ړ��������������W�i�E�B���h�E���̑��΍��W�j
	//	//int yPos = windowWH.y; //�ړ��������������W�i�E�B���h�E���̑��΍��W�j
	//	//WINDOWINFO windowInfo;
	//	////�E�B���h�E�̈ʒu���擾
	//	//windowInfo.cbSize = sizeof(WINDOWINFO);
	//	//GetWindowInfo(hwnd, &windowInfo);
	//	////�}�E�X�̈ړ���̐�΍��W�i���j�^�[���ォ��̍��W�j
	//	//xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//�Ȃ񂩂���Ă邩�璼��
	//	//yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //�E�B���h�E�̃^�C�g���o�[�̕��i31px�j���v���X
	//	////�}�E�X�̈ړ��ʂ��擾
	//	//MouseMove = Vector2(0, 0);
	//	//MouseMove = (Vector2(mousePosition.y,mousePosition.x ) - Vector2(windowWH.y, windowWH.x));//���W���ŉ�]���Ă���֌W�ł����Ȃ�(X��Y������ւ�)
	//	//mouseMoved += Vector2(MouseMove.x, MouseMove.y);
	//	//MouseMove = MouseMove / 100;
	//	//SetCursorPos(xPos_absolute, yPos_absolute);
	//}


	//POINT mousePosition;
	//GetCursorPos(&mousePosition);

	////�N���C�A���g�G���A���W�ɕϊ�����
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);
	////�r���[�|�[�g�s�� (������ɂ���)
	//Matrix4 Viewport =
	//{ windowWH.x,0,0,0,
	//0,-windowWH.y,0,0,
	//0,0,1,0,
	//windowWH.x, windowWH.y,0,1 };

	////�r���[�v���W�F�N�V�����r���[�|�[�g����
	//Matrix4 matVPV = viewProjection.matView * viewProjection.matProjection * Viewport;

	////�����s��̋t�s����v�Z����
	//Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);
	////�X�N���[�����W
	//Vector3 posNear = Vector3(mousePosition.x + mouseMoved.x, mousePosition.y + mouseMoved.y, 0);
	//Vector3 posFar = Vector3(mousePosition.x + mouseMoved.x, mousePosition.y + mouseMoved.y, 1);

	////�X�N���[�����W�n���烏�[���h���W�n��
	//posNear = AffinTrans::DivVecMat(posNear, matInverseVPV);
	//posFar = AffinTrans::DivVecMat(posFar, matInverseVPV);

	////�}�E�X���C�̕���
	//Vector3 mouseDirection = posFar - posNear;
	//mouseDirection = Vector3Normalize(mouseDirection);
	////�J��������Ə��I�u�W�F�N�g�̋���
	//const float kDistanceTestObject = 80.0f;
	//worldTransform3DReticle_.translation_ = AffinTrans::AddVector3(posNear, mouseDirection * kDistanceTestObject);

	//
	////�s��X�V
	//AffinTrans::affin(worldTransform3DReticle_);
	//worldTransform3DReticle_.TransferMatrix();

	//debugText_->SetPos(50, 70);
	//debugText_->Printf(
	//	"target:(%f,%f,%f)", mousePosition.x + mouseMoved.x, mousePosition.y + mouseMoved.y,
	//	mousePosition.y + mouseMoved.y);

#pragma endregion

#pragma region //�W���C�X�e�B�b�N�ł̃��e�B�N��

	////�X�v���C�g�̌��ݍ��W���擾
	//Vector2 spritePosition = sprite2DReticle_->GetPosition();

	//XINPUT_STATE joyState;

	////�W���C�X�e�B�b�N��Ԏ擾
	//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
	//	spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
	//	//�X�v���C�g�̍��W�ύX�𔽉f
	//	sprite2DReticle_->SetPosition(spritePosition);
	//}

#pragma endregion


}

void Player::Update(const ViewProjection& viewProjection) {

	Move();
	//�e���ˏ���
	//���˃^�C�}�[�J�E���g�_�E��
	BulletTimer--;
	//���e�B�N��
	ReticleUpdate(viewProjection);

	if (isShake == 1) {
		shakeTime--;
		if (shakeTime < 0) {
			isShake = 0;
		}
	}

	//���ԂɒB������
	if (BulletTimer <= 0) {
		Attack(viewProjection);				//�e�𔭎�
		BulletTimer = 20;	//���˃^�C�}�[��������
	}

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update(viewProjection);
	}
}

void Player::Draw(ViewProjection viewProjection_) {

	playerModel_->Draw(worldTransform_, viewProjection_);
	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}


void Player::Attack(const ViewProjection& viewProjection_) {
	if (input_->IsTriggerMouse(0) || input_->IsPressMouse(0)) {

		audio->PlayWave(shotHandol, false, 0.4f);

		//�e�̑��x
		const float kBulletSpeed = 4.0f;
		Vector3 velocity(0, 0, kBulletSpeed);


		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = bVelocity(velocity, worldTransform_);

		//���@����Ə��I�u�W�F�N�g�ւ̃x�N�g��
		velocity = viewProjection_.target - viewProjection_.eye;
		velocity = Vector3Normalize(velocity) * kBulletSpeed;

		//�e�𐶐���������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->worldSet(&worldTransform_);
		//�e���J�����ʒu�ɃZ�b�g
		newBullet->Initialize(modelBullet_, AffinTrans::GetWorldTransform(worldTransform_.parent_->matWorld_), velocity, texture2dame, texture4dame);

		//�e�̓o�^
		bullets_.push_back(std::move(newBullet));
	}
#pragma region //�W���C�X�e�B�b�N�ł̒e�o��
	//XINPUT_STATE joyState;
	////�Q�[���p�b�h���ڑ��Ȃ�Ή�������������
	//if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	return;
	//}
	////R�g���K�[�������Ă�����
	//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
	//	//�e�̑��x
	//	const float kBulletSpeed = 1.0f;
	//	Vector3 velocity(0, 0, kBulletSpeed);

	//	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	//	velocity = bVelocity(velocity, worldTransform_);

	//	//���@����Ə��I�u�W�F�N�g�ւ̃x�N�g��
	//	velocity = AffinTrans::GetWorldTransform(worldTransform3DReticle_.matWorld_) - AffinTrans::GetWorldTransform(worldTransform_.matWorld_);
	//	velocity = Vector3Normalize(velocity) * kBulletSpeed;

	//	//�e�𐶐���������
	//	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//	newBullet->worldSet(&worldTransform_);
	//	newBullet->Initialize(modelBullet_, AffinTrans::GetWorldTransform(worldTransform_.matWorld_), velocity);

	//	//�e�̓o�^
	//	bullets_.push_back(std::move(newBullet));
	//}
#pragma endregion
}

void Player::DrawSprite() {
	sprite2DReticle_->Draw();

	if (DrawHpSelect == 0) {
		if (playerHp == 7) {
			spriteHP_FULL->Draw();
		}
		else if (playerHp == 6) {
			spriteHP_1->Draw();
		}
		else if (playerHp == 5) {
			spriteHP_2->Draw();
		}
		else if (playerHp == 4) {
			spriteHP_3->Draw();
		}
		else if (playerHp == 3) {
			spriteHP_4->Draw();
		}
		else if (playerHp == 2) {
			spriteHP_5->Draw();
		}
		else if (playerHp == 1) {
			spriteHP_6->Draw();
		}
		else {
			spriteHP_7->Draw();
		}
	}

}

void Player::OnCollision() {
	audio->PlayWave(damegeHandol, false, 0.4f);
	playerHp--;
	isShake = 1;
	shakeTime = 30;
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

int Player::GetShake() {
	return isShake;
}

int Player::GetPlayerHp() {
	return playerHp;
}

void Player::worldReticleSet(WorldTransform* worldTransform) {

	worldTransform_.parent_ = worldTransform;

}


void Player::SetTarget(Vector3 Target) {

	target_ = Target;

}

void Player::setDrawSelect(int x) {
	DrawHpSelect = x;
}