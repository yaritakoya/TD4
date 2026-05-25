#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Boss.h"
#include "MyMath.h"
#include <vector>

//ゲームシーン
class GameScene
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//デストラクタ
	~GameScene();

	//表示ブロック
	void GenerateBlocks();

private:

	//ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	//カメラ
	KamataEngine::Camera camera_;

	//デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	//ブロック用のワールドトランスフォーム
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	//マップチップフィールド
	MapChipField* mapChipField_;

	//--------------------3Dモデル----------------------//
	KamataEngine::Model* modelBlock_ = nullptr;

	//デバックカメラ有効
	bool isDebugCameraActive_ = false;

	//boss
	//クラス
	Boss* boss_ = nullptr;
	//モデル
	KamataEngine::Model* boss_model_ = nullptr;
	//アタックモデル
	KamataEngine::Model* boss_at_model_ = nullptr;
	//ポジション
	//KamataEngine::Vector3 bossPosition_ = { 0.0f, 1.0f, 0.0f };
	//アタックポジション
	//KamataEngine::Vector3 attackPosition_ = { 0.0f, 1.0f, 1.0f };
};

