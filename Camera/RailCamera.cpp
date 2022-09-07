#include "RailCamera.h"

using namespace MathUtility;
#define PI 3.141592;

void RailCamera::Initialize(const Vector3 pos, const Vector3 rad) {
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rad;
	worldTransform_.Initialize();

	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.Initialize();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	viewProjection_.fovAngleY = 1.1f;
	cameraMoveY = { 0,0,0 };
	MousePosInitialize();
}

void RailCamera::Update() {

	//FOV��������
	if (viewProjection_.fovAngleY > 1.1f) {
		viewProjection_.fovAngleY -= 0.01f;
	}

	//�J�����̈ړ��̑���
	float kCharacterSpeed = 0.2f;
	//�J�����̈ړ��x�N�g��
	Vector3 cameraDownMove = { 0, 0, 0 };
	Vector3 cameraMoveX = { 0, 0, 0 };
	Vector3 cameraMoveZ = { 0, 0, 0 };

	Vector3 cameraMove = { 0, 0, 0 };
	// �}�E�X�J�[�\���̔�\��
	ShowCursor(FALSE);

#pragma region //�}�E�X�ŃJ��������
	//�J�����̉�]�x�N�g��
	Vector3 rotat = { 0, 0, 0 };
	//�J�����̈ړ��̑���
	const float cameraSpeed = 0.0005f;

	Vector2 windowWH = Vector2(WinApp::GetInstance()->kWindowWidth / 2, WinApp::GetInstance()->kWindowHeight / 2);
	POINT mousePosition;
	//�}�E�X���W(�X�N���[�����W)���擾����
	GetCursorPos(&mousePosition);

	//�N���C�A���g�G���A���W�ɕϊ�����
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	int xPos_absolute, yPos_absolute;

	int xPos = windowWH.x;  //�ړ��������������W�i�E�B���h�E���̑��΍��W�j
	int yPos = windowWH.y; //�ړ��������������W�i�E�B���h�E���̑��΍��W�j

	WINDOWINFO windowInfo;
	//�E�B���h�E�̈ʒu���擾
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//�}�E�X�̈ړ���̐�΍��W�i���j�^�[���ォ��̍��W�j
	xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//�Ȃ񂩂���Ă邩�璼��
	yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //�E�B���h�E�̃^�C�g���o�[�̕��i31px�j���v���X

	//�}�E�X�̈ړ��ʂ��擾
	MouseMove = Vector2(0, 0);
	MouseMove = (Vector2(mousePosition.y, mousePosition.x) - Vector2(windowWH.y, windowWH.x));//���W���ŉ�]���Ă���֌W�ł����Ȃ�(X��Y������ւ�)
	mouseMoved += Vector2(MouseMove.x, MouseMove.y) / 1000;
	SetCursorPos(xPos_absolute, yPos_absolute);

	worldTransform_.rotation_ = Vector3(mouseMoved.x, mouseMoved.y, 0);

	//�s��X�V
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();
#pragma endregion

	if (cameraMoveFlag == 0) {
		if (input_->PushKey(DIK_LSHIFT)) {
			kCharacterSpeed = kCharacterSpeed + 0.15;
			if (viewProjection_.fovAngleY < 1.3) {
				viewProjection_.fovAngleY += 0.03f;
			}
		}

		//�����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_S)) {
			cameraMoveZ.z = -kCharacterSpeed;
		}
		else if (input_->PushKey(DIK_W)) {
			cameraMoveZ.z = kCharacterSpeed;
		}
		if (input_->PushKey(DIK_A)) {
			cameraMoveX.x = -kCharacterSpeed;
		}
		else if (input_->PushKey(DIK_D)) {
			cameraMoveX.x = kCharacterSpeed;
		}

		if (input_->PushKey(DIK_SPACE)) {
			cameraMoveY += {0, 0.00225f, 0};
		}
	}

	cameraMove += cameraMoveX + cameraMoveZ;
	cameraMove = bVelocity(cameraMove, worldTransform_);
	worldTransform_.translation_ += cameraMove;
	worldTransform_.translation_ += cameraMoveY;

	if (worldTransform_.translation_.y < -20) {
		cameraMoveY = { 0, 0, 0 };
		worldTransform_.translation_.y = -20;
	}
	else {
		cameraMoveY += { 0, -0.0014f, 0 };
	}
	//�s��X�V
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();

	viewProjection_.eye = worldTransform_.translation_;

	if (isShake == 1) {
		viewProjection_.eye += Vector3(rand() % 4, rand() % 4, rand() % 4);
	}


	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = AffinTrans::MatVector(worldTransform_.matWorld_, forward);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;
	Target = AffinTrans::vector3Normalize(viewProjection_.target);
	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	viewProjection_.up = AffinTrans::MatVector(worldTransform_.matWorld_, up);


	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}

ViewProjection RailCamera::GetViewProjection() {
	return viewProjection_;
}

WorldTransform* RailCamera::GetWorldTransform() {
	return &worldTransform_;
}


Vector3 RailCamera::bVelocity(Vector3 velocity, WorldTransform& worldTransform) {

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

void RailCamera::MousePosInitialize() {

	Vector2 windowWH = Vector2(WinApp::GetInstance()->kWindowWidth / 2, WinApp::GetInstance()->kWindowHeight / 2);
	//�N���C�A���g�G���A���W�ɕϊ�����
	HWND hwnd = WinApp::GetInstance()->GetHwnd();

	int xPos_absolute, yPos_absolute;
	int xPos = windowWH.x;  //�ړ��������������W�i�E�B���h�E���̑��΍��W�j
	int yPos = windowWH.y; //�ړ��������������W�i�E�B���h�E���̑��΍��W�j

	WINDOWINFO windowInfo;
	//�E�B���h�E�̈ʒu���擾
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//�}�E�X�̈ړ���̐�΍��W�i���j�^�[���ォ��̍��W�j
	xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//�Ȃ񂩂���Ă邩�璼��
	yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //�E�B���h�E�̃^�C�g���o�[�̕��i31px�j���v���X

	SetCursorPos(xPos_absolute, yPos_absolute);
}

Vector3 RailCamera::GetTarget() {
	return Target;
}

void RailCamera::cameraSet(int x) {
	cameraMoveFlag = x;
}

void RailCamera::setShake(int x) {
	isShake = x;
}