#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
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

};

