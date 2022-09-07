#include"Boss.h"
#include"Player/Player.h"
#include <cmath>
#include <GameScene.h>
using namespace MathUtility;

void Boss::Initialize(Model* model,Vector3 vector3,Model* handModel,Model* minModel) {
	//NULLポインタチェック
	model_ = model;
	handModel_ = handModel;
	minModel_ = minModel;
	//シングルインスタンスを取得する
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransformBoss_.Initialize();
	worldTransformBossRightHand_.Initialize();
	worldTransformBossLeftHand_.Initialize();

	worldTransformBoss_.translation_ = vector3;

	worldTransformBossRightHand_.translation_ = worldTransformBoss_.translation_ - Vector3(50, 50, 30);
	worldTransformBossRightHand_.rotation_ = Vector3(0, -1.6f, 0);
	worldTransformBossRightHand_.matWorld_ *= AffinTrans::Rotation(worldTransformBossRightHand_.rotation_, 2);
	worldTransformBossRightHand_.scale_ = Vector3(3, 3, 3);
	worldTransformBossRightHand_.matWorld_ *= AffinTrans::Scale(worldTransformBossRightHand_.scale_);

	worldTransformBossLeftHand_.translation_ = worldTransformBoss_.translation_ - Vector3(-50, 50, 30);
	worldTransformBossLeftHand_.rotation_ = Vector3(0, -1.4f, 0);
	worldTransformBossLeftHand_.matWorld_ *= AffinTrans::Rotation(worldTransformBossLeftHand_.rotation_, 2);
	worldTransformBossLeftHand_.scale_ = Vector3(3, 3, 3);
	worldTransformBossLeftHand_.matWorld_ *= AffinTrans::Scale(worldTransformBossLeftHand_.scale_);

	keep = worldTransformBossLeftHand_.translation_;

	AffinTrans::affin(worldTransformBoss_);
	worldTransformBoss_.TransferMatrix();

	AffinTrans::affin(worldTransformBossRightHand_);
	worldTransformBossRightHand_.TransferMatrix();

	AffinTrans::affin(worldTransformBossLeftHand_);
	worldTransformBossLeftHand_.TransferMatrix();

	phaseTimer = 100;
	BulletTimer = 180;
	bossHP = 200;
}

void Boss::Move() {


	if (upDown == 0) {
		worldTransformBoss_.translation_ += Vector3(0, 0.05, 0);
		worldTransformBossRightHand_.translation_ += Vector3(0, 0.05, 0);
		worldTransformBossLeftHand_.translation_ += Vector3(0, 0.05, 0);
		if (worldTransformBoss_.translation_.y > 100) {
			upDown = 1;
		}
	}
	else if (upDown == 1) {
		worldTransformBoss_.translation_ -= Vector3(0, 0.05, 0);
		worldTransformBossRightHand_.translation_ -= Vector3(0, 0.05, 0);
		worldTransformBossLeftHand_.translation_ -= Vector3(0, 0.05, 0);
		if (worldTransformBoss_.translation_.y < 50) {
			upDown = 0;
		}
	}

	worldTransformBoss_.matWorld_ *= AffinTrans::Move(worldTransformBoss_.translation_);

	AffinTrans::affin(worldTransformBoss_);
	worldTransformBoss_.TransferMatrix();

	AffinTrans::affin(worldTransformBossRightHand_);
	worldTransformBossRightHand_.TransferMatrix();

	AffinTrans::affin(worldTransformBossLeftHand_);
	worldTransformBossLeftHand_.TransferMatrix();

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	case Phase::Up:
		Up();
		break;
	case Phase::Near:
		Near();
		break;
	case Phase::Back:
		Back();
		break;
	}
}

void Boss::Update(const Vector3 worldTransform) {

	if (phaseTimer > 0) {
		phaseTimer--;
	}
	Move();

	//発射タイマーカウントダウン
	BulletTimer--;
	//時間に達したら
	if (BulletTimer <= 0) {
		if (enemyAttackFlag == 0) {
			Attack(worldTransform);			//弾を発射
			BulletTimer = 20;	//発射タイマーを初期化
		}
	}

	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"BossHp:(%d)", bossHP);
}

void Boss::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransformBoss_, viewProjection_);
	handModel_->Draw(worldTransformBossRightHand_, viewProjection_);
	handModel_->Draw(worldTransformBossLeftHand_, viewProjection_);
}

void Boss::Approach() {

	if (phaseTimer == 0) {
		phase_ = Phase::Leave;
		phaseTimer = 300;
	}

}

void Boss::Leave() {

	if (worldTransformBossLeftHand_.translation_.z > 250) {
		Vector3 upHand = Vector3(-100, 150, 250) - worldTransformBossLeftHand_.translation_;
		upHand = AffinTrans::vector3Normalize(upHand) * 1.0f;
		worldTransformBossLeftHand_.translation_ += upHand;
	}
	else {
		Vector3 downHand = keep - worldTransformBossLeftHand_.translation_;
		downHand = AffinTrans::vector3Normalize(downHand) * 1.0f;
		worldTransformBossLeftHand_.translation_ += downHand;
	}

	if (phaseTimer == 0) {
		phase_ = Phase::Up;
		phaseTimer = 600;
	}

}

void Boss::Up() {

	worldTransformBossLeftHand_.translation_ = keep;
	phase_ = Phase::Near;
}

void Boss::Near() {



}

void Boss::Back() {

}

void Boss::OnCollisionBoss() {
	bossHP--;
	bossHP--;
	if (bossHP < 0) {
		isDead_ = true;
	}
}

void Boss::OnCollisionRight() {
	bossHP--;
	bossHP--;
	bossHP--;
	bossHP--;
	if (bossHP < 0) {
		isDead_ = true;
	}
}

void Boss::OnCollisionLeft() {
	bossHP--;
	bossHP--;
	bossHP--;
	bossHP--;
	if (bossHP < 0) {
		isDead_ = true;
	}
}



void Boss::Attack(const Vector3 worldTransform) {

	//弾の速度
	const float kBulletSpeed = 2.2f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = bVelocity(velocity, worldTransformBossRightHand_);

	Vector3 PlayerVec = worldTransform;
	PlayerVec -= Vector3(2, 0, 0);
	Vector3 EnemyVec = worldTransformBossRightHand_.translation_;

	Vector3 A_BVec = Vector3(PlayerVec.x - EnemyVec.x, PlayerVec.y - EnemyVec.y, PlayerVec.z - EnemyVec.z);
	float nomalize = sqrt(A_BVec.x * A_BVec.x + A_BVec.y * A_BVec.y + A_BVec.z * A_BVec.z);
	A_BVec = Vector3(A_BVec.x / nomalize, A_BVec.y / nomalize, A_BVec.z / nomalize);
	MovePhase = A_BVec;
	MovePhase = bVelocity(MovePhase, worldTransformBossRightHand_);
	velocity = A_BVec * kBulletSpeed;

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(minModel_, worldTransformBossRightHand_.translation_, velocity);

	//弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
	////リストに登録する
	//enemyBullets_.push_back(std::move(enemyBullet));

}

Vector3 Boss::bVelocity(Vector3& velocity, WorldTransform& worldTransform) {

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


Vector3 Boss::GetWorldPositionBoss() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransformBoss_.matWorld_.m[3][0];
	worldPos.y = worldTransformBoss_.matWorld_.m[3][1];
	worldPos.z = worldTransformBoss_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Boss::GetWorldPositionRight() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransformBossRightHand_.matWorld_.m[3][0];
	worldPos.y = worldTransformBossRightHand_.matWorld_.m[3][1];
	worldPos.z = worldTransformBossRightHand_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Boss::GetWorldPositionLeft() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransformBossLeftHand_.matWorld_.m[3][0];
	worldPos.y = worldTransformBossLeftHand_.matWorld_.m[3][1];
	worldPos.z = worldTransformBossLeftHand_.matWorld_.m[3][2];

	return worldPos;
}


void Boss::attackSet(int x) {
	enemyAttackFlag = x;
}


int Boss::GetBossHp() {

	return bossHP;
}