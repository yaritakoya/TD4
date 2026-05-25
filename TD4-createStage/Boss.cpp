#include "Boss.h"
#include "MyMath.h"
#include <random>
#include <cassert>
#include <numbers>
#include <cmath>

// 02_06の29枚目(CameraControllerのUpdate)で必要
const Vector3 operator*(const Vector3& v1, const float f) {
	Vector3 temp(v1);
	return temp *= f;
}

// 02_06のCameraControllerのUpdate/Reset関数で必要
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

// 02_06のスライド24枚目のLerp関数
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) { return Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t)); }

Vector3& operator+=(Vector3& lhv, const Vector3& rhv) {
	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;
	return lhv;
}

Vector3& operator-=(Vector3& lhv, const Vector3& rhv) {
	lhv.x -= rhv.x;
	lhv.y -= rhv.y;
	lhv.z -= rhv.z;
	return lhv;
}

Vector3& operator*=(Vector3& v, float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

Vector3& operator/=(Vector3& v, float s) {
	v.x /= s;
	v.y /= s;
	v.z /= s;
	return v;
}

Matrix4x4 MakeIdentityMatrix() {
	static const Matrix4x4 result{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {

	Matrix4x4 result{ scale.x, 0.0f, 0.0f, 0.0f, 0.0f, scale.y, 0.0f, 0.0f, 0.0f, 0.0f, scale.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	return result;
}

Matrix4x4 MakeRotateXMatrix(float theta) {
	float sin = std::sin(theta);
	float cos = std::cos(theta);

	Matrix4x4 result{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos, sin, 0.0f, 0.0f, -sin, cos, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	return result;
}

Matrix4x4 MakeRotateYMatrix(float theta) {
	float sin = std::sin(theta);
	float cos = std::cos(theta);

	Matrix4x4 result{ cos, 0.0f, -sin, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sin, 0.0f, cos, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	return result;
}

Matrix4x4 MakeRotateZMatrix(float theta) {
	float sin = std::sin(theta);
	float cos = std::cos(theta);

	Matrix4x4 result{ cos, sin, 0.0f, 0.0f, -sin, cos, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, translate.x, translate.y, translate.z, 1.0f };

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {

	// スケーリング行列の作成
	Matrix4x4 matScale = MakeScaleMatrix(scale);

	Matrix4x4 matRotX = MakeRotateXMatrix(rot.x);
	Matrix4x4 matRotY = MakeRotateYMatrix(rot.y);
	Matrix4x4 matRotZ = MakeRotateZMatrix(rot.z);
	// 回転行列の合成
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	// 平行移動行列の作成
	Matrix4x4 matTrans = MakeTranslateMatrix(translate);

	// スケーリング、回転、平行移動の合成
	Matrix4x4 matTransform = matScale * matRot * matTrans;

	return matTransform;
}

Matrix4x4& operator*=(Matrix4x4& lhm, const Matrix4x4& rhm) {
	Matrix4x4 result{};

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += lhm.m[i][k] * rhm.m[k][j];
			}
		}
	}
	lhm = result;
	return lhm;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = m1;

	return result *= m2;
}

// ワールドトランスフォーム更新(02_03の最後)
void WorldTransformUpdate(WorldTransform& worldTransform) {

	Matrix4x4 affin_mat = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);

	worldTransform.matWorld_ = affin_mat;

	// 定数バッファに転送する
	worldTransform.TransferMatrix();
}

float Lerp(float x1, float x2, float t) { return (1.0f - t) * x1 + t * x2; }

float EaseInOut(float x1, float x2, float t) {
	float easedT = -(std::cosf(std::numbers::pi_v<float> *t) - 1.0f) / 2.0f;

	return Lerp(x1, x2, easedT);
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && // x軸
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && // y軸
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);   // z軸
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result; // w=1がデカルト座標系であるので(x,y,1)のベクトルとしてmatrixとの積をとる
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f); // ベクトルに対して基本的な操作を行う行列でwが0になることはありえない
	// w=1がデカルト座標系であるので、w除算することで同次座標をデカルト座標に戻す
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

void Boss::Initialize(Model* model, Camera* camera, const Vector3& position)
{
	// NULLチェック
	assert(model);

	// メンバ変数の追加
	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// 角度調整
	//worldTransform_.rotation_.y = std::numbers::pi_v<float> *3.0f / 2.0f;
}

void Boss::Update()
{

	// =======================================================
	// パターン0：【移動中】の処理
	// =======================================================
	if (bossState_ == 0) {

		// 3秒タイマーを進めて、時間が来たら次の目標をランダム決定
		moveTimer_ += 1.0f;
		if (moveTimer_ >= moveInterval_) {
			moveTimer_ = 0.0f;

			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());
			std::uniform_int_distribution<int> dist(2, 8);

			bossTargetPosition_.x = static_cast<float>(dist(engine)) * kBlockSize;
			bossTargetPosition_.z = static_cast<float>(dist(engine)) * kBlockSize;
		}

		// 直線移動のベクトル計算
		float toTargetX = bossTargetPosition_.x - bossPosition_.x;
		float toTargetZ = bossTargetPosition_.z - bossPosition_.z;
		float distance = std::sqrtf(toTargetX * toTargetX + toTargetZ * toTargetZ);

		if (distance > bossMoveSpeed_) {
			// 目標に向かって直線移動
			bossPosition_.x += (toTargetX / distance) * bossMoveSpeed_;
			bossPosition_.z += (toTargetZ / distance) * bossMoveSpeed_;
		} else {
			// ──★ 目的地に到着した瞬間の処理 ──
			bossPosition_.x = bossTargetPosition_.x;
			bossPosition_.z = bossTargetPosition_.z;

			// ① 状態を「1: 攻撃中」に切り替える
			bossState_ = 1;

			// ② 前に作った「SPACEキーを押した時」の初期化処理をここで実行する！
			isAttacking_ = true;  // 攻撃フラグをON
			frameCount_ = 0.0f;   // 攻撃タイマーをリセット
			attackAngle_ = 0.0f;  // 角度をリセット
		}

		// 移動中も攻撃の座標はボスの足元についてこさせる
		attackPosition_ = bossPosition_;
	}
	// =======================================================
	// パターン1：【攻撃中（その場で停止）】の処理
	// =======================================================
	else if (bossState_ == 1) {

		if (isAttacking_) {
			// （中略：以前と同じ回転の計算処理）
			frameCount_ += 1.0f;
			float t = frameCount_ / maxFrame_;

			if (t >= 1.0f) {
				t = 1.0f;
				isAttacking_ = false; // 1周まわりきったら攻撃終了
				attackWaitTimer_ = 0.0f;
			} else {
				float easedT = t * t;
				attackAngle_ = -(easedT * (3.141592f * 2.0f));
				attackPosition_.x = bossPosition_.x + std::sinf(attackAngle_) * attackRadius_;
				attackPosition_.z = bossPosition_.z + std::cosf(attackAngle_) * attackRadius_;
			}
		} else {
			// ── 1周まわり終わったあとの処理 ──
			attackPosition_ = bossPosition_; // 攻撃の位置をリセット

			// 攻撃が終わってからの余韻タイマーを進める
			attackWaitTimer_ += 1.0f;
			if (attackWaitTimer_ >= 30.0f) { // 0.5秒待ったら

				// ① 状態を「0: 移動中」に戻す
				bossState_ = 0;
				moveTimer_ = 0.0f; // タイマーリセット

				// ★【ここが超重要！】
				// 移動中に戻るこの瞬間に、次の新しい目的地をランダムで決定する！
				std::random_device seed_gen;
				std::mt19937 engine(seed_gen());
				std::uniform_int_distribution<int> dist(2, 8);

				bossTargetPosition_.x = static_cast<float>(dist(engine)) * kBlockSize;
				bossTargetPosition_.z = static_cast<float>(dist(engine)) * kBlockSize;
			}
		}
	}

	// =======================================================
	// ③ 確定した座標をトランスフォームに適用して行列更新（共通）
	// =======================================================
	// ボスの行列更新
	worldTransform_.translation_.x = bossPosition_.x;
	worldTransform_.translation_.z = bossPosition_.z;
	WorldTransformUpdate(worldTransform_);

	// 攻撃の行列更新
	atWorldTransform_.translation_ = attackPosition_;
	WorldTransformUpdate(atWorldTransform_);

	// 1. ボスの座標（bossPosition_）に、毎フレーム移動スピードを「足し算」していく
	//bossPosition_.x += moveSpeed_.x;
	//bossPosition_.y += moveSpeed_.y; // YやZも動かす場合は同様に
	//bossPosition_.z += moveSpeed_.z;

	// 2. 足し算して新しくなった最新の座標を、ワールドトランスフォームに代入する
	worldTransform_.translation_ = bossPosition_;

	Vector3 targetPosition = { 5.0f,1.0f,5.0f };
	Vector3 direction = { 0.0f,0.0f,0.0f };

	// 1. ボスからターゲットへの「向き（ベクトル）」を計算する
	direction.x = targetPosition.x - bossPosition_.x;
	direction.z = targetPosition.z - bossPosition_.z;

	// 2. atan2f を使って、Y軸まわりの回転角度を計算する
	// ※C++の仕様で、atan2fは (Zの距離, Xの距離) の順番で引数を渡します
	Vector3 bossRotation_ = { 0.0f,0.0f,0.0f };
	bossRotation_.y = std::atan2f(direction.x, direction.z);

	// 3. 計算した角度をワールドトランスフォームの rotation に代入する
	worldTransform_.rotation_.y = bossRotation_.y;

	WorldTransformUpdate(worldTransform_);

}

void Boss::Draw()
{
	//モデルの描画
	model_->Draw(worldTransform_, *camera_);
}

void Boss::atInitialize(Model* model, Camera* camera, const Vector3& position)
{
	// NULLチェック
	assert(model);

	// メンバ変数の追加
	atModel_ = model;
	atCamera_ = camera;
	atWorldTransform_.Initialize();
	atWorldTransform_.translation_ = position;
	// 角度調整
	//atWorldTransform_.rotation_.y = std::numbers::pi_v<float> *3.0f / 2.0f;
}

void Boss::atUpdate()
{

	// ==========================================
	// 1. SPACEキーが押されたら、攻撃をリセットして発射
	// ==========================================
	// 「今SPACEが押されて、1フレーム前は押されていなかった（トリガー入力）」かつ「攻撃中ではない」とき
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		if (!isAttacking_) {
			isAttacking_ = true;  // 攻撃開始！
			frameCount_ = 0.0f;   // 時間を0にリセット
		}
	}

	// ==========================================
	// 2. 状態（攻撃中か、待機中か）に応じた処理
	// ==========================================
	if (isAttacking_) {
		// --- 攻撃中の処理（回転させる） ---

		// 時間を進めて進行度 t（0.0 ～ 1.0）を計算
		//frameCount_ += 1.0f;
		float t = frameCount_ / maxFrame_;

		// 1周（tが1.0以上）したら攻撃終了
		if (t >= 1.0f) {
			t = 1.0f;
			isAttacking_ = false; // 攻撃終了（フラグを下げる）

			// 【位置のリセット】ボスの中心に戻しておく
			attackPosition_ = bossPosition_;
			atWorldTransform_.translation_ = attackPosition_;
		} else {
			// イージング（緩急）をかける
			float easedT = t * t;

			// 角度の計算（逆回転）
			// ※前回PIでエラーが出た場合は「3.141592f」に置き換えてください
			attackAngle_ = -(easedT * (PI * 2.0f));

			// ボスの位置を基準に、回転座標を計算
			attackPosition_.x = bossPosition_.x + -(std::sinf(attackAngle_)) * attackRadius_;
			attackPosition_.z = bossPosition_.z + -(std::cosf(attackAngle_)) * attackRadius_;

			// 座標をトランスフォームに適用
			atWorldTransform_.translation_ = attackPosition_;
		}

	} else {
		// --- 待機中の処理（消えている間） ---

		// ボスが動いても置いていかれないように、常にボスの中心に座標をリセットし続ける
		attackPosition_ = bossPosition_;
		atWorldTransform_.translation_ = attackPosition_;
	}

	//ワールドトランスフォームの更新
	WorldTransformUpdate(atWorldTransform_);
}

void Boss::atDraw()
{
	if (isAttacking_ == true) {
		atModel_->Draw(atWorldTransform_, *atCamera_);
	}
}
