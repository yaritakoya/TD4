#pragma once
#include "KamataEngine.h"
#include <string>
#include <vector>

enum class MapChipType
{
    kBlank,//空白
    kBlock,//ブロック
};
//マップチップデータ
struct MapChipData
{
    std::vector<std::vector<MapChipType>> data;
};

class MapChipField
{
public:

    //リセット
    void ResetMapChipData();
    //読み込み
    void LoadMapChipCsv(const std::string& filePath);

    //マップチップ種別の取得
    MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
    //マップチップ座標の取得
    KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

    uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }
    uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

    // ステージ中央を取得
    KamataEngine::Vector3 GetCenterPosition();

private:

    MapChipData mapChipData_;

    //1ブロックのサイズ
    static inline const float kBlockWidth = 1.0f;
    static inline const float kBlockHeight = 1.0f;
    //ブロックの個数
    static inline const uint32_t kNumBlockVirtical = 20;
    static inline const uint32_t kNumBlockHorizontal = 100;

};

