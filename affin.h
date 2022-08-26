#pragma once

#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <math/Vector4.h>
#include <3d/WorldTransform.h>
#include "ViewProjection.h"
#include"WinApp.h"

namespace AffinTrans {

	Matrix4 Initialize();
	Matrix4 Scale(Vector3 scale);
	Matrix4 Rotation(Vector3 rotation, int X_1_Y_2_Z_3_XYZ_6);
	Matrix4 Move(Vector3 move);
	Vector3 DebugMove(Matrix4 Move);

	//ベクトルと行列の掛け算(出力Vector3)
	Vector3 MatVector(Matrix4 matrix4, Vector3 vector3);

	Vector3 GetWorldTransform(Matrix4 matrix4);
	const Vector3 AddVector3(const Vector3 v1, const Vector3 v2);
	Vector3 DivVecMat(const Vector3& vector3, const Matrix4& matrix4);

	Matrix4 setViewportMat(WorldTransform& worldTransform, WinApp* window, const Vector3& v);

	Matrix4 MatrixInverse(Matrix4& pOut);

	void  affin(WorldTransform& affin);

}; // namespace AffinTrans