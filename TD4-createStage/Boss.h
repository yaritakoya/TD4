#pragma once
#include "KamataEngine.h"
#include <3d/WorldTransform.h>

using namespace KamataEngine;

// 円周率
const float PI = 3.141592654f;

struct AABB {
	Vector3 min;
	Vector3 max;
};

// 02_06のCameraControllerのUpdate/Reset関数で必要
const Vector3 operator+(const Vector3& lhv, const Vector3& rhv);

// 02_06のスライド24枚目のLerp関数
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

// 02_06 スライド29枚目で追加
const Vector3 operator*(const Vector3& v1, const float f);

// 代入演算子オーバーロード
Vector3& operator+=(Vector3& lhs, const Vector3& rhv);
Vector3& operator-=(Vector3& lhs, const Vector3& rhv);
Vector3& operator*=(Vector3& v, float s);
Vector3& operator/=(Vector3& v, float s);

// 単位行列の作成
Matrix4x4 MakeIdentityMatrix();
// スケーリング行列の作成
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 回転行列の作成
Matrix4x4 MakeRotateXMatrix(float theta);
Matrix4x4 MakeRotateYMatrix(float theta);
Matrix4x4 MakeRotateZMatrix(float theta);
// 平行移動行列の作成
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// アフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// 代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& lhm, const Matrix4x4& rhm);

// 2項演算子オーバーロード
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

// ワールドトランスフォーム更新(02_03の最後)
void WorldTransformUpdate(WorldTransform& worldTransform);

float Lerp(float x1, float x2, float t);

float EaseInOut(float x1, float x2, float t);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

class Boss
{
public:
	//bossの初期化、更新、描画
	void Initialize(Model* model, Camera* camera, const Vector3& position);
	void Update();
	void Draw();
	//bossの攻撃の初期化、更新、描画
	void atInitialize(Model* model, Camera* camera, const Vector3& position);
	void atUpdate();
	void atDraw();
	//void surroundingAttack(const Vector3& bossPosition, const Vector3& attackPosition);

	//ボスポジション
	Vector3 bossPosition_ = { 0.0f,1.0f,0.0f };
	//アタックポジション
	Vector3 attackPosition_ = { 0.0f,1.0f,1.0f };
private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;
	//移動スピード
	Vector3 moveSpeed_ = {0.01f,0.01f,0.01f};

	// アタックワールドトランスフォーム
	WorldTransform atWorldTransform_;
	// アタックモデル
	Model* atModel_ = nullptr;
	// アタックカメラ
	Camera* atCamera_ = nullptr;
	// アタック中の有無
	bool isAttacking_ = false;

	//アタックスピード
	float attackSpeed_ = 1.0f / 60.0f;
	//アタックの角度
	float attackAngle_ = 0.0f;
	//アタックの半径
	float attackRadius_ = 1.0f;
	float frameCount_ = 0.0f; // 現在の経過時間（フレーム数）
	float maxFrame_ = 90.0f;  // 1周にかける時間（90フレーム = 1.5秒。数値を小さくすると速くなります）
	bool isDead_ = false;     // 1周したらtrueになるフラグ
	// 移動変数
	float moveTimer_ = 0.0f;     // 時間を計るタイマー
	float moveInterval_ = 180.0f; // 次の目標を決める間隔（180フレーム = 3秒）
	Vector3 bossTargetPosition_; // ★新設：次に目指すランダムな目標座標
	float bossMoveSpeed_ = 0.1f; // ★新設：じわじわ動くスピード（お好みで調整）
	const float kBlockSize = 1.0f; // 1マスのサイズ（環境に合わせて調整）
	int bossState_ = 0;        // ★新設：0=移動中、1=攻撃中
	float attackWaitTimer_ = 0.0f; // ★新設：攻撃が終わったあとの余韻（タイマー）
};

