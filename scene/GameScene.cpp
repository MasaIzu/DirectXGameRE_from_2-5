#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>


GameScene::GameScene() {}

GameScene::~GameScene() {
	// delete sprite;
	delete model_;
	delete player_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();


	textureHandle_ = TextureManager::Load("Black.png");
	ReticleTextureHandle_ = TextureManager::Load("aim.png");
	model_ = Model::Create();
	EnemyModel_ = Model::CreateFromOBJ("UFO", true);
	//3Dモデルの作成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	//Ground
	modelGround_ = Model::CreateFromOBJ("Ground", true);
	//select
	selectModelGround_ = Model::CreateFromOBJ("SelectGround", true);
	modelDome_= Model::CreateFromOBJ("SelectSkydome", true);
	//ボス
	BossModel_ = Model::CreateFromOBJ("boss", true);
	BossHandModel_ = Model::CreateFromOBJ("bossHand", true);

	//enemyHP用テクスチャ取得
	enemyTextureHandle_ = TextureManager::Load("HPbar/enemyHP.png");
	enemy50TextureHandle_ = TextureManager::Load("HPbar/enemyHP_50.png");
	nextTexture_ = TextureManager::Load("tobu.png");
	finTexture_ = TextureManager::Load("touboe.png");
	loserTexture_ = TextureManager::Load("Loser.png");
	dame2Texture_ = TextureManager::Load("2dame.png");
	dame4Texture_ = TextureManager::Load("4dame.png");


	//スプライト生成
	nextSprite.reset(Sprite::
		Create(nextTexture_, Vector2(WinApp::GetInstance()->kWindowWidth / 2, WinApp::GetInstance()->kWindowHeight / 2),
			Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	//スプライト生成
	finSprite.reset(Sprite::
		Create(finTexture_, Vector2(WinApp::GetInstance()->kWindowWidth / 2, WinApp::GetInstance()->kWindowHeight / 2),
			Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

	//スプライト生成
	loserSprite.reset(Sprite::
		Create(loserTexture_, Vector2(WinApp::GetInstance()->kWindowWidth / 2, WinApp::GetInstance()->kWindowHeight / 2),
			Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));


	worldTransformDame_.Initialize();


	//自キャラの生成
	player_ = new Player();
	//Skydome
	skydome_ = new Skydome();
	//レールカメラ
	railCamera_ = new RailCamera();
	//Ground
	ground_ = new Ground();
	//Select
	select_ = new Select();
	//ボス
	boss_ = new Boss();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//自キャラの初期化
	player_->Initialize(model_);
	//レールカメラの初期化
	railCamera_->Initialize(Vector3(0, -23, -50), Vector3(0.2, 0, 0));
	//skydomeの初期化
	skydome_->Initialize(modelSkydome_);
	//groundの初期化
	ground_->Initialize(modelGround_);
	//Selectの初期化
	select_->Initialize(selectModelGround_, modelDome_);
	//ボス
	boss_->Initialize(BossModel_, Vector3(0,70,300),BossHandModel_, EnemyModel_);

	//ファイルの読み込み
	LoadEnemyPopData();

}

void GameScene::Update() {

	if (sceneChange > 0) {
		sceneChange--;
	}
	
	
	switch (scene_) {
	case Scene::First://タイトル
	default:
		//描画切り替え
		changeDraw = 0;
		
		//レールカメラ
		railCamera_->setShake(player_->GetShake());
		railCamera_->cameraSet(0);
		railCamera_->Update();
		//行列の再計算
		viewProjection_.UpdateMatrix();

		//自キャラの更新
		player_->setDrawSelect(1);
		player_->SetTarget(railCamera_->GetTarget());
		player_->worldReticleSet(railCamera_->GetWorldTransform());
		player_->Update(railCamera_->GetViewProjection());

		if (input_->TriggerKey(DIK_RETURN)) {
			sceneChange = 180;
			scene_ = Scene::Second;
		}
		break;
	case Scene::Second://次への準備
		//描画切り替え
		changeDraw = 1;
		if (sceneChange == 0) {

			secondInitialize();
			DeadEnemys = 0;
			scene_ = Scene::Third;
		}

		break;
	case Scene::Third://ステージ2
		//描画切り替え
		changeDraw = 2;

		if (player_->GetPlayerHp() <= 0) {
			scene_ = Scene::Loser;
		}

		//レールカメラ
		railCamera_->setShake(player_->GetShake());
		railCamera_->Update();
		//行列の再計算
		viewProjection_.UpdateMatrix();

		//自キャラの更新
		player_->setDrawSelect(0);
		player_->SetTarget(railCamera_->GetTarget());
		player_->worldReticleSet(railCamera_->GetWorldTransform());
		player_->Update(railCamera_->GetViewProjection());

		UpdateEnemyPopCommands_phase1();
		//敵発生コマンドー
		if (DeadEnemys > 9) {
			UpdateEnemyPopCommands_phase2();
		}
		if (DeadEnemys > 26) {
			UpdateEnemyPopCommands_phase3();
		}

		enemyUpdate(0);

		CheckAllCollisions();

		if (DeadEnemys == 60) {
			scene_ = Scene::Four;
			enemybay();
			DeadEnemys = 0;
		}

		break;

	case Scene::Four://語りかけ
		//描画切り替え
		changeDraw = 3;

		railCamera_->cameraSet(1);
		railCamera_->Update();

		player_->setDrawSelect(0);
		player_->SetTarget(railCamera_->GetTarget());
		player_->worldReticleSet(railCamera_->GetWorldTransform());
		player_->Update(railCamera_->GetViewProjection());


		scene_ = Scene::Boss;
		sceneChange = 300;
		secondInitialize();
		break;
	case Scene::Boss://ボス
		//描画切り替え
		changeDraw = 4;

		if (player_->GetPlayerHp() <= 0) {
			scene_ = Scene::Loser;
		}

		if (sceneChange == 0) {
			UpdateEnemyPopCommands_phase4();
		}
		railCamera_->setShake(player_->GetShake());
		railCamera_->cameraSet(0);
		railCamera_->Update();

		player_->setDrawSelect(0);
		player_->SetTarget(railCamera_->GetTarget());
		player_->worldReticleSet(railCamera_->GetWorldTransform());
		player_->Update(railCamera_->GetViewProjection());

		boss_->SetGameScene(this);
		boss_->Update(player_->GetWorldPosition());

		enemyUpdate(0);

		CheckAllCollisions();

		if (boss_->GetBossHp() < 0) {
			secondInitialize();
			scene_ = Scene::Back;
		}


		break;
	case Scene::Back://ty&タイトルへ
		//描画切り替え
		changeDraw = 5;
		enemybay();
		if (input_->TriggerKey(DIK_RETURN)) {
			scene_ = Scene::First;
			secondInitialize();
		}

		break;

	case Scene::Loser://エース
		//描画切り替え
		changeDraw = 6;
		enemybay();
		if (input_->TriggerKey(DIK_RETURN)) {
			secondInitialize();
			scene_ = Scene::First;
		}

		break;
	}



//デバッグ用表示
#pragma region debugText
	/*debugText_->SetPos(50, 70);
	debugText_->Printf(
		"target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
		viewProjection_.target.z);*/

	/*debugText_->SetPos(50, 90);
	debugText_->Printf(
		"up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);*/
#pragma endregion
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	if (changeDraw == 0) {
		select_->Draw(railCamera_->GetViewProjection());
	}
	if (changeDraw == 2 || changeDraw == 3 || changeDraw == 4) {
		skydome_->Draw(railCamera_->GetViewProjection());
		ground_->Draw(railCamera_->GetViewProjection());
	}
	if (changeDraw == 4) {
		boss_->Draw(railCamera_->GetViewProjection());
	}

	player_->Draw(railCamera_->GetViewProjection());

	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		enemy_->Draw(railCamera_->GetViewProjection());
	}

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw(railCamera_->GetViewProjection(), textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawSprite();

	if (changeDraw == 1) {
		nextSprite->Draw();
	}
	if (changeDraw == 5) {
		finSprite->Draw();
	}
	if (changeDraw == 6) {
		loserSprite->Draw();
	}


	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	//判定対象のAとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		float a = posB.x - posA.x;
		float b = posB.y - posA.y;
		float c = posB.z - posA.z;

		float d = sqrt(a * a + b * b + c * c);

		if (d <= playerRadius + enemyBulletRadius) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//敵キャラの座標
	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		posA = enemy_->GetWorldPosition();

		//自キャラと敵弾すべての当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//自弾の座標
			posB = bullet->GetWorldPosition();

			float a = posB.x - posA.x;
			float b = posB.y - posA.y;
			float c = posB.z - posA.z;

			float d = sqrt(a * a + b * b + c * c);

			if (d <= playerBulletRadius + enemyRadius) {

				//敵キャラの衝突時コールバックを呼び出す
				enemy_->OnCollision();

				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();

				DeadEnemys++;
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵Bossの当たり判定

	posA = boss_->GetWorldPositionBoss();

	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自弾の座標
		posB = bullet->GetWorldPosition();

		float a = posB.x - posA.x;
		float b = posB.y - posA.y;
		float c = posB.z - posA.z;

		float d = sqrt(a * a + b * b + c * c);

		if (d <= playerBulletRadius + bossRadius) {

			//敵キャラの衝突時コールバックを呼び出す
			boss_->OnCollisionBoss();

			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();

		}
	}

#pragma endregion

#pragma region 自弾と敵BossHandの当たり判定

	posA = boss_->GetWorldPositionRight();

	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自弾の座標
		posB = bullet->GetWorldPosition();

		float a = posB.x - posA.x;
		float b = posB.y - posA.y;
		float c = posB.z - posA.z;

		float d = sqrt(a * a + b * b + c * c);

		if (d <= playerBulletRadius + bossHandRadius) {

			//敵キャラの衝突時コールバックを呼び出す
			boss_->OnCollisionRight();

			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();

		}
	}

#pragma endregion

#pragma region 自弾と敵BossHandの当たり判定

	posA = boss_->GetWorldPositionLeft();

	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自弾の座標
		posB = bullet->GetWorldPosition();

		float a = posB.x - posA.x;
		float b = posB.y - posA.y;
		float c = posB.z - posA.z;

		float d = sqrt(a * a + b * b + c * c);

		if (d <= playerBulletRadius + bossHandRadius) {

			//敵キャラの衝突時コールバックを呼び出す
			boss_->OnCollisionLeft();

			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();

		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	//自キャラと敵弾すべての当たり判定
	//for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
	//	for (const std::unique_ptr<EnemyBullet>& bullet2 : enemyBullets) {

	//		//自弾の座標
	//		posA = bullet->GetWorldPosition();

	//		//敵弾の座標
	//		posB = bullet2->GetWorldPosition();

	//		float a = posB.x - posA.x;
	//		float b = posB.y - posA.y;
	//		float c = posB.z - posA.z;

	//		float d = sqrt(a * a + b * b + c * c);

	//		if (d <= playerBulletRadius + enemyBulletRadius) {
	//			//自キャラの衝突時コールバックを呼び出す
	//			bullet->OnCollision();
	//			//敵弾の衝突時コールバックを呼び出す
	//			bullet2->OnCollision();
	//		}
	//	}
	//}

#pragma endregion
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet) {
	//リストに登録する
	enemyBullets_.push_back(std::move(enemyBullet));
}

void GameScene::LoadEnemyPopData(){

	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop_phase1.csv");

	assert(file.is_open());

	// バッファをクリアします。
	enemyPopCommand_phase1.str("");
	// 状態をクリアします。
	enemyPopCommand_phase1.clear(std::stringstream::goodbit);

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommand_phase1 << file.rdbuf();

	//ファイルを閉じる
	file.close();

	file.open("Resources/enemyPop_phase2.csv");

	assert(file.is_open());

	// バッファをクリアします。
	enemyPopCommand_phase2.str("");
	// 状態をクリアします。
	enemyPopCommand_phase2.clear(std::stringstream::goodbit);

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommand_phase2 << file.rdbuf();

	//ファイルを閉じる
	file.close();

	//ファイルを開く
	file.open("Resources/enemyPop_phase3.csv");

	assert(file.is_open());

	// バッファをクリアします。
	enemyPopCommand_phase3.str("");
	// 状態をクリアします。
	enemyPopCommand_phase3.clear(std::stringstream::goodbit);

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommand_phase3 << file.rdbuf();

	//ファイルを閉じる
	file.close();

	//ファイルを開く
	file.open("Resources/enemyPop_phase4.csv");

	assert(file.is_open());

	// バッファをクリアします。
	enemyPopCommand_phase4.str("");
	// 状態をクリアします。
	enemyPopCommand_phase4.clear(std::stringstream::goodbit);

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommand_phase4 << file.rdbuf();

	//ファイルを閉じる
	file.close();

}


void GameScene::UpdateEnemyPopCommands_phase1() {
	//待機処理
	if (isStand_) {
		standTime_--;
		if (standTime_ <= 0) {
			//待機完了
			isStand_ = false;
		}
		return;
	}
	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommand_phase1, line)) {
		//1行分の文字数をストリームに変換して解折しやすくなる
		std::istringstream line_stream(line);
		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			std::getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// Y座標
			std::getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// Z座標
			std::getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			MakeEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isStand_ = true;
			standTime_ = waitTime;

			//ループを抜ける
			break;
		}
	}
}

void GameScene::UpdateEnemyPopCommands_phase2() {
	//待機処理
	if (isStand_) {
		standTime_--;
		if (standTime_ <= 0) {
			//待機完了
			isStand_ = false;
		}
		return;
	}
	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommand_phase2, line)) {
		//1行分の文字数をストリームに変換して解折しやすくなる
		std::istringstream line_stream(line);
		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			std::getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// Y座標
			std::getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// Z座標
			std::getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			MakeEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isStand_ = true;
			standTime_ = waitTime;

			//ループを抜ける
			break;
		}
	}
}

void GameScene::UpdateEnemyPopCommands_phase3() {
	//待機処理
	if (isStand_) {
		standTime_--;
		if (standTime_ <= 0) {
			//待機完了
			isStand_ = false;
		}
		return;
	}
	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommand_phase3, line)) {
		//1行分の文字数をストリームに変換して解折しやすくなる
		std::istringstream line_stream(line);
		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			std::getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// Y座標
			std::getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// Z座標
			std::getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			MakeEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isStand_ = true;
			standTime_ = waitTime;

			//ループを抜ける
			break;
		}
	}
}

void GameScene::UpdateEnemyPopCommands_phase4() {
	//待機処理
	if (isStand_) {
		standTime_--;
		if (standTime_ <= 0) {
			//待機完了
			isStand_ = false;
		}
		return;
	}
	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommand_phase4, line)) {
		//1行分の文字数をストリームに変換して解折しやすくなる
		std::istringstream line_stream(line);
		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			std::getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// Y座標
			std::getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// Z座標
			std::getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			MakeEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isStand_ = true;
			standTime_ = waitTime;

			//ループを抜ける
			break;
		}
	}
}


void GameScene::MakeEnemy(Vector3 EnemyPos){

	//敵キャラの生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	//敵キャラの初期化
	newEnemy->Initialize(EnemyModel_, enemyTextureHandle_, enemy50TextureHandle_, EnemyPos, model_);

	//敵キャラにアドレスを渡す
	newEnemy->SetPlayer(player_);

	//リストに登録する
	enemys_.push_back(std::move(newEnemy));

}


void GameScene::secondInitialize() {

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//自キャラの初期化
	player_->Initialize(model_);
	//レールカメラの初期化
	railCamera_->Initialize(Vector3(0, 0, -50), Vector3(0.2, 0, 0));
	//ボス
	boss_->Initialize(BossModel_, Vector3(0, 70, 300), BossHandModel_, EnemyModel_);
	LoadEnemyPopData();
}


void GameScene::enemyUpdate(int x) {
	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});
	//敵キャラの更新
	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		enemy_->attackSet(x);
		enemy_->SetGameScene(this);
		enemy_->Update();
	}

	//デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});
	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}
}

void GameScene::enemybay() {
	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
		});
	//敵キャラの座標
	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		enemy_->OnCollision();
	}
	//デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});
	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->OnCollision();
	}
}