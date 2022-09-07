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
	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();

	int xPos_absolute, yPos_absolute;
	int xPos = windowWH.x;  //移動させたいｘ座標（ウィンドウ内の相対座標）
	int yPos = windowWH.y; //移動させたいｙ座標（ウィンドウ内の相対座標）

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス

	SetCursorPos(xPos_absolute, yPos_absolute);
}

Vector3 Reticle::GetAngle(){
	Vector3 Angle{ angle.x,angle.y,0 };

	return Angle;
}

Vector3 Reticle::GetWorldPosition() {

	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
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