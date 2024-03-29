#pragma once

#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <3d/WorldTransform.h>

namespace AffinTrans {

	Matrix4 Initialize();
	Matrix4 Scale(Vector3 scale);
	Matrix4 Rotation(Vector3 rotation, int X_1_Y_2_Z_3_XYZ_6);
	Matrix4 Move(Vector3 move);
	Vector3 DebugMove(Matrix4 Move);
	void  affin(WorldTransform& affin);

	//ワールド座標変換用
	Vector3 Vec3Scale(Vector3& scale);
	Vector3 Vec3Rotation(Vector3& scale);
	Vector3 Vec3Move(Vector3& scale);
}; // namespace AffinTrans