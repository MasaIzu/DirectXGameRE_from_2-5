#include "RailCamera.h"

using namespace MathUtility;
#define PI 3.141592;

void RailCamera::Initialize(const Vector3 pos, const Vector3 rad) {
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rad;
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	viewProjection_.fovAngleY = 1.1f;
	cameraMoveY = { 0,0,0 };
	MousePosInitialize();
}

void RailCamera::Update() {

	//FOVをいじる
	if (viewProjection_.fovAngleY > 1.1f) {
		viewProjection_.fovAngleY -= 0.01f;
	}

	//カメラの移動の速さ
	float kCharacterSpeed = 0.2f;
	//カメラの移動ベクトル
	Vector3 cameraDownMove = { 0, 0, 0 };
	Vector3 cameraMoveX = { 0, 0, 0 };
	Vector3 cameraMoveZ = { 0, 0, 0 };

	Vector3 cameraMove = { 0, 0, 0 };
	// マウスカーソルの非表示
	ShowCursor(FALSE);

#pragma region //マウスでカメラ操作
	//カメラの回転ベクトル
	Vector3 rotat = { 0, 0, 0 };
	//カメラの移動の速さ
	const float cameraSpeed = 0.0005f;

	Vector2 windowWH = Vector2(WinApp::GetInstance()->kWindowWidth / 2, WinApp::GetInstance()->kWindowHeight / 2);
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

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

	//マウスの移動量を取得
	MouseMove = Vector2(0, 0);
	MouseMove = (Vector2(mousePosition.y, mousePosition.x) - Vector2(windowWH.y, windowWH.x));//座標軸で回転している関係でこうなる(XとYが入れ替え)
	mouseMoved += Vector2(MouseMove.x, MouseMove.y) / 1000;
	SetCursorPos(xPos_absolute, yPos_absolute);

	worldTransform_.rotation_ = Vector3(mouseMoved.x, mouseMoved.y, 0);

	//行列更新
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

		//押した方向で移動ベクトルを変更
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
	//行列更新
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();

	viewProjection_.eye = worldTransform_.translation_;

	if (isShake == 1) {
		viewProjection_.eye += Vector3(rand() % 4, rand() % 4, rand() % 4);
	}


	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = AffinTrans::MatVector(worldTransform_.matWorld_, forward);
	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;
	Target = AffinTrans::vector3Normalize(viewProjection_.target);
	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	viewProjection_.up = AffinTrans::MatVector(worldTransform_.matWorld_, up);


	//ビュープロジェクションを更新
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

Vector3 RailCamera::GetTarget() {
	return Target;
}

void RailCamera::cameraSet(int x) {
	cameraMoveFlag = x;
}

void RailCamera::setShake(int x) {
	isShake = x;
}