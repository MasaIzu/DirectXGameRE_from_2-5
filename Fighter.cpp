#include "Fighter.h"
#include "MathUtility.h"
using namespace MathUtility;


void Fighter::Initialize(){

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,-2.5,3 };

	//çsóÒçXêV
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();
}

void Fighter::Update(){


	//èÊÇËï®
	Move();

}

void Fighter::Move(){

}

void Fighter::Draw(){



}


Vector3 Fighter::bVelocity(Vector3 velocity, WorldTransform& worldTransform) {

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