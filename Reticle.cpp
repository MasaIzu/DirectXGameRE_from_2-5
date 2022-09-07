#include "Reticle.h"
#include "MathUtility.h"
#include <math.h>
using namespace MathUtility;

#define PI 3.141592;

void Reticle::Initialize() {

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	MousePosInitialize();
}

void Reticle::Update(const ViewProjection& viewProjection) {
	ReticleUpdate(viewProjection);
}




void Reticle::MousePosInitialize(){

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

Vector3 Reticle::GetAngle(){
	Vector3 Angle{ angle.x,angle.y,0 };

	return Angle;
}

Vector3 Reticle::GetWorldPosition() {

	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��ړ��������擾(���[���h���W)
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}


WorldTransform* Reticle::GetWorldTransform() {
	return &worldTransform3DReticle_;
}


void Reticle::worldTransformSet(WorldTransform* worldTransform) {

	worldTransform3DReticle_.parent_ = worldTransform;

}